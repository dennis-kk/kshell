#ifndef CALLER_BUNDLER_ACTIVATOR_H
#define CALLER_BUNDLER_ACTIVATOR_H

#include "kshell.h"

/**
 * @brief bundle activator implementation
 */
class BundleActivator_CallerBundle : public BundleActivator {
public:
    /**
     * @brief ctor
     */
    BundleActivator_CallerBundle();

    /**
     * @brief dtor
     */
    virtual ~BundleActivator_CallerBundle();

    /**
     * @brief will be called when bundle starting
     */
    virtual void start(BundleContext* context) throw(BundleException);

    /**
     * @brief will be called when bundle stopping
     */
    virtual void stop(BundleContext* context) throw(BundleException);

public:
    /**
     * @brief get context
     */
    BundleContext* getContext();

private:
    BundleContext* _context;
};

#endif // CALLER_BUNDLER_ACTIVATOR_H
