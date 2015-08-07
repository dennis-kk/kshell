#ifndef BUNDLE_H
#define BUNDLE_H

#include "pragma.h"

#include "bundle_guid.h"
#include "bundle_exception.h"
#include "class_not_found_exception.h"

class BundleContext;
class Service;
class ServiceReference;

/**
 * @brief Bundle - service container
 */
class Bundle {
public:
	static const int UNINSTALLED = 1; ///< Bundle was de-installed - reserved for future
	static const int INSTALLED = 2;   ///< Bundle was installed
	static const int RESOLVED = 4;    ///< Bundle was resolved - reserved for future
	static const int STARTING = 8;    ///< Bundle was starting
	static const int STOPPING = 16;   ///< Bundle was stopping
	static const int ACTIVE = 32;     ///< Bundle was actived
    static const int STOPPED = 64;    ///< Bundle was stopped

public:
    virtual ~Bundle() {}
    /**
     * @brief get BundleContext of current bundle
     */
    virtual BundleContext* getBundleContext() = 0;

    /**
     * @brief get Bundle's GUID
     */
	virtual BundleGuid getBundleId() = 0;

    /**
     * @brief get Bundle's version string
     */
    virtual const std::string& getVersion() = 0;

    /**
     * @brief get Bundle's location
     */
	virtual const std::string& getLocation() = 0;

    /**
     * @brief get Bundle's state
     */
	virtual int state() = 0;

    /**
     * @brief get Bundle's symbolic name
     */
	virtual const std::string& getSymbolicName() = 0;

    /**
     * @brief create a new service from bundle
     * @param name service name
     * @return ServiceReference
     */
	virtual ServiceReference* loadClass(const std::string& name) throw(ClassNotFoundException) = 0;

    /**
     * @brief start a bundle
     */
	virtual void start() throw(BundleException) = 0;

    /**
     * @brief stop a bundle
     */
	virtual void stop() throw(BundleException) = 0;
};

#endif // BUNDLE_H
