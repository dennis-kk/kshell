#ifndef BUNDLE_LOADER_H
#define BUNDLE_LOADER_H

#include "pragma.h"

#include "scope_lock.h"
#include "bundle.h"
#include "bundle_handle.h"

class Service;
class BundleActivator;

class BundleLoader {
public:
	/**
	 * ctor
	 */
	BundleLoader();

	/**
	 * dtor
	 */
	~BundleLoader();

	/**
	 * load a bundle
	 *
	 * @param path the directory which contais the class
	 * @param name bundle name
	 * @return bundle handle
	 */
    void load(const std::string& path, std::string name) throw(BundleException);

	/**
	 * free a bundle
	 */
	void unload() throw(BundleException);

    /**
     * fork a Service by service name
     */
    Service* newClass(const std::string& entry, std::string name) throw(ClassNotFoundException);

    /**
     * destroy a Service by service name
     */
    void destroyClass(const std::string& entry, Service* service, std::string name);

    /**
     * fork a BundleActivator
     */
    BundleActivator* newActivator(const std::string& entry) throw(ClassNotFoundException);

	/**
	 * test whether the handle is valid
	 *
	 * @param handle bundle handle
	 * @return true on yes, false on no
	 */
	bool isValid();

private:
    OsHandle _handle;
    RecursiveLock _lock;
};

#endif // BUNDLE_LOADER_H
