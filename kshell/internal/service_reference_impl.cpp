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
