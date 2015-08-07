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
