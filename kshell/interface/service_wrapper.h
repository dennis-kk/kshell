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

#ifndef SERVICE_WRAPPER_H
#define SERVICE_WRAPPER_H

#include <stdexcept>
#include <typeinfo>

#include "service_exception.h"
#include "service.h"

template <typename T>
class ServiceWrapper : public Service, public T {
public:
    ServiceWrapper() {}
    virtual ~ServiceWrapper() {}
    #include "service_wrapper_impl.inl"
};

template <typename T, typename P>
class ServiceWrapperParam1 : public Service, public T {
public:
    ServiceWrapperParam1(P p) : T(p) {}
    virtual ~ServiceWrapperParam1() {}
    #include "service_wrapper_impl.inl"
};

template <typename T, typename P1, typename P2>
class ServiceWrapperParam2 : public Service, public T {
public:
    ServiceWrapperParam2(P1 p1, P2 p2) : T(p1, p2) {}
    virtual ~ServiceWrapperParam2() {}
    #include "service_wrapper_impl.inl"
};

template <typename T, typename P1, typename P2, typename P3>
class ServiceWrapperParam3 : public Service, public T {
public:
    ServiceWrapperParam3(P1 p1, P2 p2, P3 p3) : T(p1, p2, p3) {}
    virtual ~ServiceWrapperParam3() {}
    #include "service_wrapper_impl.inl"
};

template <typename T, typename P1, typename P2, typename P3, typename P4>
class ServiceWrapperParam4 : public Service, public T {
public:
    ServiceWrapperParam4(P1 p1, P2 p2, P3 p3, P4 p4) : T(p1, p2, p3, p4) {}
    virtual ~ServiceWrapperParam4() {}
    #include "service_wrapper_impl.inl"
};

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5>
class ServiceWrapperParam5 : public Service, public T {
public:
    ServiceWrapperParam5(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) : T(p1, p2, p3, p4, p5) {}
    virtual ~ServiceWrapperParam5() {}
    #include "service_wrapper_impl.inl"
};

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class ServiceWrapperParam6 : public Service, public T {
public:
    ServiceWrapperParam6(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) : T(p1, p2, p3, p4, p5, p6) {}
    virtual ~ServiceWrapperParam6() {}
    #include "service_wrapper_impl.inl"
};

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
class ServiceWrapperParam7 : public Service, public T {
public:
    ServiceWrapperParam7(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) : T(p1, p2, p3, p4, p5, p6, p7) {}
    virtual ~ServiceWrapperParam7() {}
    #include "service_wrapper_impl.inl"
};

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
class ServiceWrapperParam8 : public Service, public T {
public:
    ServiceWrapperParam8(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) : T(p1, p2, p3, p4, p5, p6, p7, p8) {}
    virtual ~ServiceWrapperParam8() {}
    #include "service_wrapper_impl.inl"
};

#endif // SERVICE_WRAPPER_H
