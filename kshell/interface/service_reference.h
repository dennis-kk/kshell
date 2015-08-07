#ifndef SERVICE_REFERENCE_H
#define SERVICE_REFERENCE_H

#include "pragma.h"

class Service;

class ServiceReference {
public:
    virtual ~ServiceReference() {}
    virtual Service* get() = 0;
    virtual void unget() = 0;
    virtual void destroy() = 0;
    virtual bool isOk() = 0;
};

#endif // SERVICE_REFERENCE_H