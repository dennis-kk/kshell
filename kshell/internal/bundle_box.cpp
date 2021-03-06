/*
 * Copyright (c) 2013-2015, dennis wang
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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

#ifndef WIN32
#   include <unistd.h>
#   include <time.h>
#endif // WIN32

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
}

bool BundleBox::waitForStop() {
    // waiting for quit
    while (!_quit) {
#   ifdef WIN32
    Sleep(100);
#   else
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = 1000000;
    nanosleep(&req, 0);
#   endif // WIN32
    }
    return _quit;
}

std::string BundleBox::getRoot() {
    return _root;
}
