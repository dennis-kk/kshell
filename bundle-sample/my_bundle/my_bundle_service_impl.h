#ifndef MY_BUNDLE_SERVICE_IMPL_H
#define MY_BUNDLE_SERVICE_IMPL_H

#include "my_bundle_service.h"

class MyServiceImpl : public MyService {
public:
	MyServiceImpl() {	
	}
	
	virtual ~MyServiceImpl() {
	}
	
	virtual const std::string& getGreeting() const {
		static std::string greeting = "hi caller!";
		return greeting;
	}
	
	void serviceStart() {
	}
	
	void serviceStop() {
	}
};

#endif // MY_BUNDLE_SERVICE_IMPL_H
