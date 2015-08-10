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

#ifndef SERVICE_REFERENCE_IMPL_H
#define SERVICE_REFERENCE_IMPL_H

#include <string>

#include "pragma.h"

#include "scope_lock.h"
#include "service_reference.h"

class Service;
class BundleContextImpl;

class ServiceReferenceImpl : public ServiceReference {
public:
    ServiceReferenceImpl();
    ServiceReferenceImpl(BundleContextImpl* context, Service* service, std::string name);
    virtual ~ServiceReferenceImpl();

    virtual Service* get();
    virtual void unget();
    virtual void destroy();
    virtual bool isOk();

    int getCount();
    Service* getNoop();
    void setNoop(Service* service);

private:
    RecursiveLock _lock;
    Service* _service;
    BundleContextImpl* _context;
    int _reference;
    std::string _name;
};

#endif // SERVICE_REFERENCE_IMPL_H
