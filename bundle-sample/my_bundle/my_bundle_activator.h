#ifndef MY_BUNDLE_ACTIVATOR_H
#define MY_BUNDLE_ACTIVATOR_H

#include "kshell.h"

/**
 * @brief bundle activator implementation
 */
class BundleActivator_MyBundle : public BundleActivator {
public:
    /**
     * @brief ctor
     */
    BundleActivator_MyBundle();

    /**
     * @brief dtor
     */
    virtual ~BundleActivator_MyBundle();

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

#endif // MY_BUNDLE_ACTIVATOR_H
