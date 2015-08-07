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
        delete iterator->second->getNoop();
        delete iterator->second;
        _bundles.erase(iterator);
        std::map<std::string, BundleGuid>::iterator iteratorName =
            _bundleNames.find(iterator->second->getNoop()->getSymbolicName());
        if (iteratorName != _bundleNames.end()) {
            _bundleNames.erase(iteratorName);
        }
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
