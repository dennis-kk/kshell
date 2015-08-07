#include "service_reference_impl.h"
#include "bundle_context_impl.h"

ServiceReferenceImpl::ServiceReferenceImpl() {
    _service = NULL;
    _context = NULL;
    _reference = 0;
    _name = "";
}

ServiceReferenceImpl::ServiceReferenceImpl(BundleContextImpl* context, Service* service,
    std::string name) {
    _service = service;
    _context = context;
    _name = name;
    _reference = 0;
}

ServiceReferenceImpl::~ServiceReferenceImpl() {
    _service = NULL;
    _reference = 0;
}

Service* ServiceReferenceImpl::get() {
    ScopeLock<RecursiveLock> guard(&_lock);
    _reference += 1;
    return _service;
}

Service* ServiceReferenceImpl::getNoop() {
    ScopeLock<RecursiveLock> guard(&_lock);
    return _service;
}

void ServiceReferenceImpl::unget() {
    ScopeLock<RecursiveLock> guard(&_lock);
    _reference -= 1;
    if (0 > _reference) {
        _reference = 0;
    }
}

void ServiceReferenceImpl::destroy() {
    do {
        ScopeLock<RecursiveLock> guard(&_lock);
        if (0 != getCount()) {
            return;
        }
    } while (0);
    _context->destroyClass(this, _name);
}

int ServiceReferenceImpl::getCount() {
    ScopeLock<RecursiveLock> guard(&_lock);
    return _reference;
}

bool ServiceReferenceImpl::isOk() {
    ScopeLock<RecursiveLock> guard(&_lock);
    return (NULL != _service);
}
