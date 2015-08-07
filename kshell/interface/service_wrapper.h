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
