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

#include "bundle_manager.h"
#include "bundle_reference_impl.h"
#include "bundle_impl.h"
#include "bundle_context_impl.h"

BundleManager* BundleManager::_instance = NULL;
RecursiveLock BundleManager::_lock;
int BundleManager::_bundleId = 1;
BundleReferenceImpl BundleManager::_invalidBundleReference;

BundleManager* BundleManager::getInstance() {
    ScopeLock<RecursiveLock> guard(&_lock);
    if (NULL == _instance) {
        _instance = new BundleManager();
    }
    return _instance;
}

void BundleManager::destory() {
    ScopeLock<RecursiveLock> guard(&_lock);
    if (NULL == _instance) {
        return;
    }
    std::map<BundleGuid, BundleReferenceImpl*>::iterator iterator =
        _instance->_bundles.begin();
    for (; iterator != _instance->_bundles.end(); iterator++) {
        delete iterator->second;
    }
    _instance->_bundles.clear();
    _instance->_bundleNames.clear();
    delete _instance;
    _instance = NULL;
}

BundleGuid BundleManager::add(BundleImpl* bundleImpl, BundleContextImpl* bundleContextImpl) {
    ScopeLock<RecursiveLock> guard(&_lock);
    BundleGuid id = _bundleId++;
    std::map<BundleGuid, BundleReferenceImpl*>::iterator iterator =
        _bundles.find(id);
    if (iterator != _bundles.end()) {
        return 0;
    }
    bundleImpl->setBundleContext(bundleContextImpl);
    BundleReferenceImpl* impl = new BundleReferenceImpl(bundleImpl);
    _bundles.insert(std::make_pair(id, impl));
    _bundleNames.insert(std::make_pair(bundleImpl->getSymbolicName(), id));
    return id;
}

void BundleManager::remove(BundleGuid id) {
    ScopeLock<RecursiveLock> guard(&_lock);
    std::map<BundleGuid, BundleReferenceImpl*>::iterator iterator =
        _bundles.find(id);
    if (iterator == _bundles.end()) {
        return;
    }
    if (0 == iterator->second->getCount()) {
        std::map<std::string, BundleGuid>::iterator iteratorName =
            _bundleNames.find(iterator->second->getNoop()->getSymbolicName());
        if (iteratorName != _bundleNames.end()) {
            _bundleNames.erase(iteratorName);
        }
        delete iterator->second->getNoop();
        delete iterator->second;
        _bundles.erase(iterator);
    }
}

bool BundleManager::has(BundleGuid id) {
    ScopeLock<RecursiveLock> guard(&_lock);
    return (_bundles.find(id) != _bundles.end());
}

bool BundleManager::has(const std::string& name) {
    ScopeLock<RecursiveLock> guard(&_lock);
    return (_bundleNames.find(name) != _bundleNames.end());
}

BundleReferenceImpl* BundleManager::get(BundleGuid id) {
    ScopeLock<RecursiveLock> guard(&_lock);
    std::map<BundleGuid, BundleReferenceImpl*>::iterator iterator =
        _bundles.find(id);
    if (iterator == _bundles.end()) {
        return &_invalidBundleReference;
    }
    return iterator->second;
}

BundleReferenceImpl* BundleManager::get(const std::string& name) {
    ScopeLock<RecursiveLock> guard(&_lock);
    std::map<std::string, BundleGuid>::iterator iteratorName =
        _bundleNames.find(name);
    if (iteratorName != _bundleNames.end()) {
        return get(iteratorName->second);
    }
    return &_invalidBundleReference;
}

std::vector<BundleReferenceImpl*> BundleManager::getBundles() {
    ScopeLock<RecursiveLock> guard(&_lock);
    std::vector<BundleReferenceImpl*> bundles;
    std::map<BundleGuid, BundleReferenceImpl*>::iterator iterator =
        _bundles.begin();
    for (; iterator != _bundles.end(); iterator++) {
        bundles.push_back(iterator->second);
    }
    return bundles;
}
