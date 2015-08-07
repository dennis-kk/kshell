#ifndef SERVICE_LISTENER_H
#define SERVICE_LISTENER_H

#include "pragma.h"

class ServiceEvent;

class ServiceListener {
public:
    virtual ~ServiceListener() {}
    virtual void serviceChanged(ServiceEvent event) = 0;
};

#endif // SERVICE_LISTENER_H
