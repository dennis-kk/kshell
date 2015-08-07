#include "bundle_reference_impl.h"

BundleReferenceImpl::BundleReferenceImpl() {
    _bundle = NULL;
    _reference = 0;
}

BundleReferenceImpl::BundleReferenceImpl(Bundle* bundle) {
    _bundle = bundle;
    _reference = 0;
}

BundleReferenceImpl::~BundleReferenceImpl() {
    _bundle = NULL;
    _reference = 0;
}

Bundle* BundleReferenceImpl::get() {
    ScopeLock<RecursiveLock> guard(&_lock);
    _reference += 1;
    return _bundle;
}

Bundle* BundleReferenceImpl::getNoop() {
    ScopeLock<RecursiveLock> guard(&_lock);
    return _bundle;
}

void BundleReferenceImpl::unget() {
    ScopeLock<RecursiveLock> guard(&_lock);
    _reference -= 1;
    if (0 > _reference) {
        _reference = 0;
    }
}

int BundleReferenceImpl::getCount() {
    ScopeLock<RecursiveLock> guard(&_lock);
    return _reference;
}

bool BundleReferenceImpl::isOk() {
    ScopeLock<RecursiveLock> guard(&_lock);
    return (NULL != _bundle);
}
