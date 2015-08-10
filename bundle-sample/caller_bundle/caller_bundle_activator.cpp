#include <iostream>

#include "caller_bundle_activator.h"
#include "my_bundle_service.h"

BundleActivator_CallerBundle::BundleActivator_CallerBundle() {
    _context = 0;    
}

BundleActivator_CallerBundle::~BundleActivator_CallerBundle() {
}

void BundleActivator_CallerBundle::start(BundleContext* context) throw(BundleException) {
    _context = context;
    // fork a 'MyService' and call it
    AutoService<MyService> myService = loadClass<MyService>(_context, "mybundle", "myservice");
    if (!myService) {
        std::cout << "create service 'myservice' failed" << std::endl;
    }
    std::cout << "call MyService::getGreeting(): " << myService->getGreeting() << std::endl;
    // destroy 'MyService'
    myService.destroy();
}

void BundleActivator_CallerBundle::stop(BundleContext* context) throw(BundleException) {
}

BundleContext* BundleActivator_CallerBundle::getContext() {
    return _context;
}
