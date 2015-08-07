#include "export.h"
#include "my_bundle_service_impl.h"
#include "my_bundle_activator.h"

static BundleActivator_MyBundle activator;

void* newActivator() {
    return &activator;
}

void* newService(const char* name) {
    if (NULL == name) {
        return 0;
    }

    // expose 'myservice'
    if (std::string("myservice") == name) {
        try {
            ServiceWrapper<MyServiceImpl>* manager =
                new ServiceWrapper<MyServiceImpl>();
            return manager->service();
        } catch (std::bad_alloc&) {
            return 0;
        }
    }

    return 0;
}

void* destroyService(void* service, const char* name) {
    if (0 == service || 0 == name) {
        return 0;
    }

    if (std::string(name) == "myservice") {
        MyServiceImpl* myService =
            ServiceWrapper<MyServiceImpl>::cast(service);

        if (NULL == myService) {
            return 0;
        }
        delete myService;
    }

    return 0;
}
