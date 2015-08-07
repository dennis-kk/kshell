#ifndef FRAMEWORK_IMPL_H
#define FRAMEWORK_IMPL_H

#include "scope_lock.h"
#include "framework.h"
#include "bundle_box.h"

class FrameworkImpl : public Framework {
public:
    FrameworkImpl();
    virtual ~FrameworkImpl();
    virtual void start() throw(FrameworkException);
    virtual void stop() throw(FrameworkException);
    virtual const std::string& getLocation();

private:
    BundleBox* _box;
    RecursiveLock _lock;
    RecursiveLock _flagLock;
    std::string _location;
    bool _flag;
};

#endif // FRAMEWORK_IMPL_H
