#ifndef MY_BUNDLE_SERVICE_H
#define MY_BUNDLE_SERVICE_H

#include <string>

#include "kshell.h"

class MyService {
public:
	virtual ~MyService() {}
	virtual const std::string& getGreeting() const = 0;
};

#endif // MY_BUNDLE_SERVICE_H
