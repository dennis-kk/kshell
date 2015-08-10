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

#include <algorithm>
#include <sstream>

#include "bundle_context_impl.h"
#include "bundle_reference_impl.h"
#include "bundle_impl.h"
#include "bundle_listener.h"
#include "bundle_activator.h"
#include "service_listener.h"
#include "service.h"
#include "bundle_manager.h"
#include "service_reference_impl.h"
#include "service_event.h"
#include "framework_factory.h"

BundleContextImpl::BundleContextImpl() {
    _state = 0;
    _id = 0;
    _location = "";
    _version = "";
    _symbolicName = "";
    _bundleImpl = NULL;
}

BundleContextImpl::~BundleContextImpl() {
}

void BundleContextImpl::addBundleListener(BundleListener* listener) {
    ScopeLock<RecursiveLock> guard(&_lock);
    std::list<BundleListener*>::iterator result = 
        std::find(_bundleListeners.begin(), _bundleListeners.end(), listener);
    if (result != _bundleListeners.end()) {
        return;
    }
    _bundleListeners.push_back(listener);
}

void BundleContextImpl::removeBundleListener(BundleListener* listener) {
    ScopeLock<RecursiveLock> guard(&_lock);
    std::list<BundleListener*>::iterator result = 
        std::find(_bundleListeners.begin(), _bundleListeners.end(), listener);
    if (result != _bundleListeners.end()) {
        _bundleListeners.erase(result);
    }
}

void BundleContextImpl::addServiceListener(ServiceListener* listener) {
    ScopeLock<RecursiveLock> guard(&_lock);
    std::list<ServiceListener*>::iterator result = 
        std::find(_serviceListeners.begin(), _serviceListeners.end(), listener);
    if (result != _serviceListeners.end()) {
        return;
    }
    _serviceListeners.push_back(listener);
}

void BundleContextImpl::removeServiceListener(ServiceListener* listener) {
    ScopeLock<RecursiveLock> guard(&_lock);
    std::list<ServiceListener*>::iterator result = 
        std::find(_serviceListeners.begin(), _serviceListeners.end(), listener);
    if (result != _serviceListeners.end()) {
        _serviceListeners.erase(result);
    }
}

BundleReference* BundleContextImpl::getBundle() {
    ScopeLock<RecursiveLock> guard(&_lock);
    if (!BundleManager::getInstance()->has(_id)) {
        return NULL;
    }
    return dynamic_cast<BundleReference*>(
        BundleManager::getInstance()->get(_id));
}

BundleReference* BundleContextImpl::getBundle(BundleGuid id) {
    ScopeLock<RecursiveLock> guard(&_lock);
    if (!BundleManager::getInstance()->has(_id)) {
        return NULL;
    }
    return dynamic_cast<BundleReference*>(
        BundleManager::getInstance()->get(id));
}

BundleReference* BundleContextImpl::getBundle(const std::string& symbolicName) {
    ScopeLock<RecursiveLock> guard(&_lock);
    if (!BundleManager::getInstance()->has(symbolicName)) {
        return NULL;
    }
    return dynamic_cast<BundleReference*>(
        BundleManager::getInstance()->get(symbolicName));
}

std::vector<BundleReference*> BundleContextImpl::getBundles() {
    ScopeLock<RecursiveLock> guard(&_lock);
    std::vector<BundleReferenceImpl*> bundleImpls = BundleManager::getInstance()->getBundles();
    std::vector<BundleReference*> bundles;
    std::vector<BundleReferenceImpl*>::iterator iterator = bundleImpls.begin();
    for (; iterator != bundleImpls.end(); iterator++) {
        bundles.push_back(dynamic_cast<BundleReference*>(*iterator));
    }
    return bundles;
}

const std::string& BundleContextImpl::getProperty(const std::string& key) {
    ScopeLock<RecursiveLock> guard(&_lock);
    return _config.getProperty(key);;
}

Framework* BundleContextImpl::getFramework() {
    return FrameworkFactory::newFramework();
}

ServiceReference* BundleContextImpl::newClass(const std::string& name) throw(ClassNotFoundException) {
    ScopeLock<RecursiveLock> guard(&_lock);
    if (!_loader.isValid()) {
        return dynamic_cast<ServiceReference*>(&_invalidServiceReference);
    }
    Service* service = _loader.newClass(_newServiceSymbol, name);
    if (NULL == service) {
        return dynamic_cast<ServiceReference*>(&_invalidServiceReference);
    }
    // fire Service::STARTING event
    fireServiceEvent(ServiceEvent(Service::STARTING, name));
    service->initialize();

    // fire Service::ACTIVE event
    fireServiceEvent(ServiceEvent(Service::ACTIVE, name));

    return dynamic_cast<ServiceReference*>(new ServiceReferenceImpl(this, service, name));
}

void BundleContextImpl::destroyClass(ServiceReferenceImpl* serviceReference, std::string name) {
    ScopeLock<RecursiveLock> guard(&_lock);
    if (!_loader.isValid()) {
        return;
    }
    // fire Service::STOPPING event
    fireServiceEvent(ServiceEvent(Service::STOPPING, name));
    serviceReference->getNoop()->finalize();

    _loader.destroyClass(_destroyServiceSymbol, serviceReference->getNoop(), name);
    // fire Service::STOPPED event
    fireServiceEvent(ServiceEvent(Service::STOPPED, name));

    delete serviceReference;
}

BundleReference* BundleContextImpl::installBundle(const std::string& location) throw(BundleException) {
    ScopeLock<RecursiveLock> guard(&_lock);

    if (_loader.isValid()) {
        return dynamic_cast<BundleReference*>(&_invalidBundleReference);
    }

    _location = location;

#   ifdef WIN32
    std::string configPath = _location + "\\meta.inf";
#   else
    std::string configPath = _location + "/meta.inf";
#   endif

    // open configuration
    if (!_config.open(configPath)) {
        return dynamic_cast<BundleReference*>(&_invalidBundleReference);
    }

    // read and save configuration
    _symbolicName          = _config.getProperty("symbolicName");
    _version               = _config.getProperty("version");
    _activatorSymbol       = _config.getProperty("activator");
    _newServiceSymbol      = _config.getProperty("newService");
    _destroyServiceSymbol  = _config.getProperty("destroyService");
    std::string bundleName = _config.getProperty("bundle");

#   ifdef WIN32
    bundleName += ".dll";
#   else
    bundleName += ".so";
#   endif

    // open bundle
    _loader.load(_location, bundleName);
    if (!_loader.isValid()) {
        std::stringstream reason;
        reason << "load bundle [" << location << "] failed";
        throw BundleException(reason.str());
    }

    // change bundle state
    _state = Bundle::INSTALLED;

    // create bundle
    _id = BundleManager::getInstance()->add(
        (_bundleImpl = new BundleImpl()), this);

    _activator = _loader.newActivator(_activatorSymbol);
    if (NULL == _activator) {
        std::stringstream reason;
        reason << "new bundle activator [" << location << "] failed";
        throw BundleException(reason.str());
    }

    // fire Bundle::INSTALLED event
    fireBundleEvent(
        BundleEvent(Bundle::INSTALLED, BundleManager::getInstance()->get(_id)));

    return BundleManager::getInstance()->get(_id);
}

BundleActivator* BundleContextImpl::getActivator() {
    ScopeLock<RecursiveLock> guard(&_lock);
    return _activator;
}

void BundleContextImpl::setState(int state) {
    ScopeLock<RecursiveLock> guard(&_lock);
    _state = state;
}

int BundleContextImpl::state() {
    ScopeLock<RecursiveLock> guard(&_lock);
    return _state;
}

void BundleContextImpl::setBundleId(BundleGuid id) {
    ScopeLock<RecursiveLock> guard(&_lock);
    _id = id;
}

BundleGuid BundleContextImpl::getBundleId() {
    ScopeLock<RecursiveLock> guard(&_lock);
    return _id;
}

void BundleContextImpl::setSymbolicName(const std::string& symbolicName) {
    ScopeLock<RecursiveLock> guard(&_lock);
    _symbolicName = symbolicName;
}

const std::string& BundleContextImpl::getSymbolicName() {
    ScopeLock<RecursiveLock> guard(&_lock);
    return _symbolicName;
}

void BundleContextImpl::setLocation(const std::string& location) {
    ScopeLock<RecursiveLock> guard(&_lock);
    _location = location;
}

void BundleContextImpl::setShellLocation(const std::string& shellLocation) {
    ScopeLock<RecursiveLock> guard(&_lock);
    _shellLocation = shellLocation;
}

const std::string& BundleContextImpl::getLocation() {
    ScopeLock<RecursiveLock> guard(&_lock);
    return _location;
}

const std::string& BundleContextImpl::getShellLocation() {
    return _shellLocation;
}

void BundleContextImpl::version(const std::string& version) {
    ScopeLock<RecursiveLock> guard(&_lock);
    _version = version;
}

const std::string& BundleContextImpl::getVersion() {
    ScopeLock<RecursiveLock> guard(&_lock);
    return _version;
}

BundleLoader* BundleContextImpl::getLoader() {
    ScopeLock<RecursiveLock> guard(&_lock);
    return &_loader;
}

void BundleContextImpl::fireBundleEvent(BundleEvent bundleEvent) {
    ScopeLock<RecursiveLock> guard(&_lock);
    if (_bundleListeners.empty()) {
        return;
    }

    std::list<BundleListener*>::iterator iterator = _bundleListeners.begin();
    for (; iterator != _bundleListeners.end(); iterator++) {
        (*iterator)->bundleChanged(bundleEvent);
    }
}

void BundleContextImpl::fireServiceEvent(ServiceEvent serviceEvent) {
    ScopeLock<RecursiveLock> guard(&_lock);
    if (_serviceListeners.empty()) {
        return;
    }

    std::list<ServiceListener*>::iterator iterator = _serviceListeners.begin();
    for (; iterator != _serviceListeners.end(); iterator++) {
        (*iterator)->serviceChanged(serviceEvent);
    }
}
