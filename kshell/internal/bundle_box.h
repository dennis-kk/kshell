#ifndef BUNDLE_BOX_H
#define BUNDLE_BOX_H

#include "pragma.h"

#include <list>
#include <map>
#include <string>

#include "scope_lock.h"
#include "condition.h"
#include "bundle_exception.h"
#include "bundle_event.h"
#include "resolver.h"
#include "bundle_box_loader.h"
#include "ignore.h"

class BundleContextImpl;

/**
 * @brief bundle container, manages the life-cycle of bundles
 */
class BundleBox {
public:
    /**
     * @brief ctor
     */
    BundleBox();

    /**
     * @brief dtor
     */
    ~BundleBox();

    /**
     * @brief start bundles locates in 'root'
     * @param root bundles' location
     */
    void start(const std::string& root = "") throw(BundleException);

    /**
     * @brief stop bundles
     */
    void stop() throw(BundleException);

    /**
     * @brief wait for stopping notification and return
     */
    bool waitForStop();

    /**
     * @brief get root directory
     */
    std::string getRoot();

private:
    std::list<std::string> _directories;
    std::map<std::string, BundleContextImpl*> _contexts;
    std::string _root;
    Resolver _resolver;
    Ignore _ignore;
    BundleBoxLoader _boxLoader;
    RecursiveLock _lock;
    Condition _signal;
    bool _quit;
    size_t _maxMargin;
};

#endif // BUNDLE_BOX_H
