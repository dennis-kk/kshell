#include <iostream>
#include <sstream>
#include <iomanip>

#include "bundle_box.h"
#include "bundle_context_impl.h"
#include "bundle_reference.h"
#include "auto_bundle.h"
#include "version.h"
#include "color.h"
#include "misc.h"
#include "ignore.h"

BundleBox::BundleBox() {
    _quit = true;
    _maxMargin = 0;
}

BundleBox::~BundleBox() {
}

void BundleBox::start(const std::string& root) throw(BundleException) {
    ScopeLock<RecursiveLock> guard(&_lock);
    if (!_quit) {
        return;
    }

    std::cout << "kshell v." << yellow << KSHELL_VERSION << white << std::endl;

    if (root.empty()) {
        _root = getExePath();
    } else {
        _root = root;
    }

    std::cout << "Root -> [" << green << _root << white << "]" << std::endl;

    std::string bundlePath = _root;

#   ifdef WIN32
    bundlePath += "\\";
    _ignore.open(bundlePath + "\\ignore");
#   else
    bundlePath += "/";
    _ignore.open(bundlePath + "/ignore");
#   endif // WIN32

    bundlePath += "bundle";

//#   ifdef WIN32
//    bundlePath += "\\";
//#   else
//    bundlePath += "/";
//#   endif // WIN32

    std::cout << "Check bundle dependency";

    _boxLoader.load(bundlePath, _resolver);
    if (NULL != _resolver.check()) {
        std::cout << " -> " << red << "Failed" << white << "]" << std::endl;
        std::stringstream reason;
        reason << "Bundles dependence is unresolved";
        throw BundleException(reason.str());
    }

    std::cout << " -> [" << green << "Resolved" << white << "]" << std::endl;

    std::vector<std::string> resolvedNames = _resolver.resolve();

    std::vector<std::string>::iterator iterator = resolvedNames.begin();
    for (; iterator != resolvedNames.end(); iterator++) {
        if (iterator->size() > _maxMargin) {
            _maxMargin = iterator->size();
        }
    }

    iterator = resolvedNames.begin();
    for (; iterator != resolvedNames.end(); iterator++) {
        if (_ignore.ignore(*iterator)) {
            std::cout << "Install Bundle -> [" << std::left << std::setw(_maxMargin)
                << yellow << *iterator << white << "]";
            std::cout << " -> ["<< red << "IGNORED" << white << "]" << std::endl;
            continue;
        }
        if (!_boxLoader.has(*iterator)) {
            std::stringstream reason;
            reason << "Install bundle [" << *iterator << "]" << " failed";
            throw BundleException(reason.str());
        }
        if (BundleManager::getInstance()->has(*iterator)) {
            // bundle already installed, skip it
            continue;
        }
        std::string path = _boxLoader.get(*iterator);
        BundleContextImpl* contextImpl = new BundleContextImpl();
        contextImpl->setShellLocation(_root);
        AutoBundle bundle(contextImpl->installBundle(path));
        if (!bundle.isOk()) {
            std::stringstream reason;
            reason << "Install bundle [" << path << "]" << " failed";
            throw BundleException(reason.str());
        }
        std::cout << "Install Bundle -> [" << std::left << std::setw(_maxMargin)
            << yellow << *iterator << white << "]";
        _contexts.insert(std::make_pair(*iterator, contextImpl));
        _directories.push_back(path);
        bundle->start();
        std::cout << " -> ["<< green << "INSTALLED" << white << "]" << std::endl;
    }

    _quit = false;
}

void BundleBox::stop() throw(BundleException) {
    ScopeLock<RecursiveLock> guard(&_lock);
    if (_quit) {
        return;
    }
    std::vector<std::string> resolvedNames = _resolver.resolve();
    std::vector<std::string>::reverse_iterator name = resolvedNames.rbegin();
    std::map<std::string, BundleContextImpl*>::iterator iterator = _contexts.end();
    for (; name != resolvedNames.rend(); name++) {
        if (_ignore.ignore(*name)) {
            // Bundle was ignored
            continue;
        }
        iterator = _contexts.find(*name);
        if (Bundle::STOPPED == iterator->second->state()) {
            // Bundle already stopped
            std::stringstream reason;
            reason << "Stop a stopped bundle [" << iterator->first << "]";
            throw BundleException(reason.str());
        }
        if (iterator == _contexts.end()) {
            std::stringstream reason;
            reason << "Stop bundle [" << iterator->first << "]" << " failed";
            throw BundleException(reason.str());
        }
        std::cout << "Stopping Bundle -> [" << std::left << std::setw(_maxMargin)
            << yellow << iterator->first << white << "]";
        AutoBundle bundle(iterator->second->getBundle());
        bundle->stop();
        std::cout << " -> [" << green << "STOPPED" << white << "]" << std::endl;
		// unload bundle
		delete iterator->second;
    }
    _contexts.clear();
    _quit = true;
    _signal.signal();
}

bool BundleBox::waitForStop() {
    ScopeLock<RecursiveLock> guard(&_lock);
    if (!_quit) {
        _signal.wait();
    }
    return _quit;
}

std::string BundleBox::getRoot() {
    return _root;
}
