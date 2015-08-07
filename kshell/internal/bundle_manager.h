#ifndef BUNDLE_MANAGER_H
#define BUNDLE_MANAGER_H

#include "pragma.h"

#include <map>
#include <string>
#include <vector>

#include "scope_lock.h"
#include "bundle_guid.h"
#include "bundle_reference_impl.h"

class BundleImpl;
class BundleReferenceImpl;
class BundleContextImpl;

class BundleManager {
public:
    static BundleManager* getInstance();
    static void destory();

    BundleGuid add(BundleImpl* bundleImpl, BundleContextImpl* bundleContextImpl);
    void remove(BundleGuid id);

    bool has(BundleGuid id);
    bool has(const std::string& name);

    BundleReferenceImpl* get(BundleGuid id);
    BundleReferenceImpl* get(const std::string& name);
    std::vector<BundleReferenceImpl*> getBundles();

protected:
    BundleManager() {}
    BundleManager(const BundleManager&) {}
    ~BundleManager() {}

private:
    std::map<BundleGuid, BundleReferenceImpl*> _bundles;
    std::map<std::string, BundleGuid> _bundleNames;
    static BundleManager* _instance;
    static RecursiveLock _lock;
    static int _bundleId;
    static BundleReferenceImpl _invalidBundleReference;
};

#endif // BUNDLE_MANAGER_H
