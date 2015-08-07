#ifndef BUNDLE_ACTIVATOR_H
#define BUNDLE_ACTIVATOR_H

#include "pragma.h"
#include "bundle_exception.h"

class BundleContext;

/**
 * @brief bundle'a activator
 */
class BundleActivator {
public:
    virtual ~BundleActivator() {}
    /**
     * @brief will be called when bundle starting
     */
    virtual void start(BundleContext* context) throw(BundleException) = 0;

    /**
     * @brief will be called when bundle stopping
     */
    virtual void stop(BundleContext* context) throw(BundleException) = 0;
};

#endif // BUNDLE_ACTIVATOR_H
