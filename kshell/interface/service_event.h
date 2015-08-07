#ifndef SERVICE_EVENT_H
#define SERVICE_EVENT_H

#include "pragma.h"

#include <string>

class ServiceReference;

class ServiceEvent {
public:
    ServiceEvent(int type, std::string name) {
        _type = type;
        _name = name;
    }

    ServiceEvent(const ServiceEvent& serviceEvent) {
        operator=(serviceEvent);
    }

    const ServiceEvent& operator=(const ServiceEvent& serviceEvent) {
        _type = serviceEvent._type;
        _name = serviceEvent._name;
        return *this;
    }

    int type() {
        return _type;
    }

    std::string getName() {
        return _name;
    }

private:
    ServiceEvent() {
        _type = 0;
        _name = "";
    }

private:
    int _type;
    std::string _name;
};

#endif // SERVICE_EVENT_H
