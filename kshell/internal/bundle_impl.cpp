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
