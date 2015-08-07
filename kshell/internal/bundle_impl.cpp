#include "bundle_impl.h"
#include "bundle_context_impl.h"
#include "bundle_event.h"
#include "bundle_activator.h"

BundleImpl::BundleImpl() {
    _contextImpl = NULL;
}

BundleImpl::~BundleImpl() {
}

BundleContext* BundleImpl::getBundleContext() {
    return dynamic_cast<BundleContext*>(_contextImpl);
}

BundleGuid BundleImpl::getBundleId() {
    if (NULL == _contextImpl) {
        return 0;
    }
    return _contextImpl->getBundleId();
}

const std::string& BundleImpl::getVersion() {
	static std::string null = "";
    if (NULL == _contextImpl) {
        return null;
    }
    return _contextImpl->getVersion();
}

const std::string& BundleImpl::getLocation() {
	static std::string null = "";
    if (NULL == _contextImpl) {
        return null;
    }
    return _contextImpl->getLocation();
}

int BundleImpl::state() {
    if (NULL == _contextImpl) {
        return 0;
    }
    return _contextImpl->state();
}

const std::string& BundleImpl::getSymbolicName() {
	static std::string null = "";
    if (NULL == _contextImpl) {
        return null;
    }
    return _contextImpl->getSymbolicName();
}

ServiceReference* BundleImpl::loadClass(const std::string& name) throw(ClassNotFoundException) {
    if (NULL == _contextImpl) {
        return NULL;
    }
    return _contextImpl->newClass(name);
}

void BundleImpl::start() throw(BundleException) {
    if (NULL == _contextImpl) {
        return;
    }

    _contextImpl->fireBundleEvent(
        BundleEvent(Bundle::STARTING, _contextImpl->getBundle()));

    _contextImpl->getActivator()->start(dynamic_cast<BundleContext*>(_contextImpl));
    _contextImpl->setState(Bundle::ACTIVE);

    _contextImpl->fireBundleEvent(
       BundleEvent(Bundle::ACTIVE, _contextImpl->getBundle()));
}

void BundleImpl::stop() throw(BundleException) {
    if (NULL == _contextImpl) {
        return;
    }
    _contextImpl->fireBundleEvent(
        BundleEvent(Bundle::STOPPING, _contextImpl->getBundle()));

    _contextImpl->getActivator()->stop(dynamic_cast<BundleContext*>(_contextImpl));
    _contextImpl->setState(Bundle::STOPPED);

    _contextImpl->fireBundleEvent(
        BundleEvent(Bundle::STOPPED, _contextImpl->getBundle()));
}

void BundleImpl::setBundleContext(BundleContextImpl* contextImpl) {
    _contextImpl = contextImpl;
}
