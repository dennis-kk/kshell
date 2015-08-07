#include "export.h"
#include "caller_bundle_activator.h"

static BundleActivator_CallerBundle activator;

void* newActivator() {
    return &activator;
}

void* newService(const char* name) {
    return 0;
}

void* destroyService(void* service, const char* name) {
    return 0;
}
