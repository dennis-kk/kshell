#include "my_bundle_activator.h"

BundleActivator_MyBundle::BundleActivator_MyBundle() {
    _context = 0;
}

BundleActivator_MyBundle::~BundleActivator_MyBundle() {
    _context = 0;
}

void BundleActivator_MyBundle::start(BundleContext* context) throw(BundleException) {
    _context = context;
}

void BundleActivator_MyBundle::stop(BundleContext* context) throw(BundleException) {
}

BundleContext* BundleActivator_MyBundle::getContext() {
    return _context;
}
