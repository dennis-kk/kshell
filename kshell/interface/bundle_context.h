#ifndef BUNDLE_CONTEXT_H
#define BUNDLE_CONTEXT_H

#include "pragma.h"

#include <string>
#include <vector>

#include "bundle_exception.h"
#include "bundle_guid.h"

class Bundle;
class BundleReference;
class FrameworkReference;
class BundleListener;
class ServiceListener;
class Framework;

/**
 * @brief bundle's context
 */
class BundleContext {
public:
    virtual ~BundleContext() {}
    /**
     * @brief add a bundle life-cycle event listener
     */
	virtual void addBundleListener(BundleListener* listener) = 0;

    /**
     * @brief remove a bundle life-cycle event listener
     */
    virtual void removeBundleListener(BundleListener* listener) = 0;

    /**
     * @brief add a service life-cycle event listener
     */
	virtual void addServiceListener(ServiceListener* listener) = 0;

    /**
     * @brief remove a service life-cycle event listener
     */
    virtual void removeServiceListener(ServiceListener* listener) = 0;

    /**
     * @brief get bundle which bundle context holds
     */
	virtual BundleReference* getBundle() = 0;

    /**
     * @brief get other bundle by bundle identifier
     */
	virtual BundleReference* getBundle(BundleGuid id) = 0;

    /**
     * @brief get other bundle by bundle symbolic name
     */
	virtual BundleReference* getBundle(const std::string& symbolicName) = 0;

    /**
     * @brief get all bundle that osgi loaded
     */
	virtual std::vector<BundleReference*> getBundles() = 0;

    /**
     * @brief get bundle's configuration value by key
     */
	virtual const std::string& getProperty(const std::string& key) = 0;

    /**
     * @brief get osgi framework instance(singleton)
     */
    virtual Framework* getFramework() = 0;

    /**
     * @breif get bundle's location
     */
    virtual const std::string& getLocation() = 0;

    /**
     * @brief get executable binary path
     */
    virtual const std::string& getShellLocation() = 0;
};

#endif // BUNDLE_CONTEXT_H
