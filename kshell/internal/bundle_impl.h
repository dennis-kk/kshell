#ifndef BUNDLE_IMPL_H
#define BUNDLE_IMPL_H

#include "pragma.h"

#include "bundle.h"

class BundleContextImpl;

class BundleImpl : public Bundle {
public:
    BundleImpl();
    virtual ~BundleImpl();

    virtual BundleContext* getBundleContext();
	virtual BundleGuid getBundleId();
	virtual const std::string& getVersion();
	virtual const std::string& getLocation();
	virtual int state();
	virtual const std::string& getSymbolicName();
	virtual ServiceReference* loadClass(const std::string& name) throw(ClassNotFoundException);
	virtual void start() throw(BundleException);
	virtual void stop() throw(BundleException);

    void setBundleContext(BundleContextImpl* contextImpl);

private:
    BundleContextImpl* _contextImpl;
};

#endif // BUNDLE_IMPL_H
