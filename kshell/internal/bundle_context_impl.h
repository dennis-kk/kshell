#ifndef BUNDLE_CONTEXT_IMPL_H
#define BUNDLE_CONTEXT_IMPL_H

#include "pragma.h"

#include <list>
#include <vector>
#include <string>

#include "bundle_context.h"
#include "service_event.h"

#include "scope_lock.h"
#include "bundle_manager.h"
#include "bundle_loader.h"
#include "bundle_event.h"
#include "bundle_config.h"

#include "bundle_reference_impl.h"
#include "service_reference_impl.h"

class BundleImpl;
class ServiceReferenceImpl;

class BundleContextImpl : public BundleContext {
public:
    BundleContextImpl();
    virtual ~BundleContextImpl();

    virtual void addBundleListener(BundleListener* listener);
    virtual void removeBundleListener(BundleListener* listener);

	virtual void addServiceListener(ServiceListener* listener);
    virtual void removeServiceListener(ServiceListener* listener);

	virtual BundleReference* getBundle();
	virtual BundleReference* getBundle(BundleGuid id);
	virtual BundleReference* getBundle(const std::string& symbolicName);
	virtual std::vector<BundleReference*> getBundles();

	virtual const std::string& getProperty(const std::string& key);
    virtual Framework* getFramework();
    virtual const std::string& getLocation();
    virtual const std::string& getShellLocation();

    //
    // functions for implementation
    //
	
    BundleReference* installBundle(const std::string& location) throw(BundleException);

    BundleActivator* getActivator();

    void setState(int state);
    int state();

    void setBundleId(BundleGuid id);
    BundleGuid getBundleId();

    void setSymbolicName(const std::string& symbolicName);
    const std::string& getSymbolicName();

    void setShellLocation(const std::string& shellLocation);
    void setLocation(const std::string& location);

    void version(const std::string& version);
    const std::string& getVersion();

    BundleLoader* getLoader();
    ServiceReference* newClass(const std::string& name) throw(ClassNotFoundException);
    void destroyClass(ServiceReferenceImpl* serviceReference, std::string name);

    void fireBundleEvent(BundleEvent bundleEvent);
    void fireServiceEvent(ServiceEvent serviceEvent);

private:
    int _state;
    BundleGuid _id;
    std::string _location;
    std::string _shellLocation;
    std::string _version;
    std::string _symbolicName;
    std::string _activatorSymbol;
    std::string _newServiceSymbol;
    std::string _destroyServiceSymbol;
    RecursiveLock _lock;
    BundleLoader _loader;
    BundleImpl* _bundleImpl;
    BundleConfig _config;
    BundleActivator* _activator;
    BundleReferenceImpl _invalidBundleReference;
    ServiceReferenceImpl _invalidServiceReference;

    std::list<BundleListener*> _bundleListeners;
    std::list<ServiceListener*> _serviceListeners;

    std::list<BundleEvent> _unfiredBundleEvent;
    std::list<ServiceEvent> _unfiredServiceEvent;

private:
    static const int MAX_UNFIRED_BUNDLE_EVENT = 1024;
    static const int MAX_UNFIRED_SERVICE_EVENT = 1024;
};

#endif // BUNDLE_CONTEXT_IMPL_H
