#ifndef SERVICE_H
#define SERVICE_H

#include "pragma.h"

#include "service_exception.h"

class Service {
public:
    static const int STARTING = 1;
    static const int ACTIVE = 2;
    static const int STOPPING = 4;
    static const int STOPPED = 8;

public:
    virtual ~Service() {}
    virtual void initialize() throw(ServiceException) = 0;
	virtual void finalize() throw(ServiceException) = 0;
};

#endif // SERVICE_H
