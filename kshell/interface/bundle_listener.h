#ifndef BUNDLE_LISTENER_H
#define BUNDLE_LISTENER_H

#include "pragma.h"

#include "bundle_event.h"

class BundleListener {
public:
    virtual ~BundleListener() {}
    virtual void bundleChanged(BundleEvent event) = 0;
};

#endif // BUNDLE_LISTENER_H
