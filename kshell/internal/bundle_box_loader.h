#ifndef BUNDLE_BOX_LOADER_H
#define BUNDLE_BOX_LOADER_H

#include <map>
#include <string>

#include "resolver.h"
#include "bundle_exception.h"

/**
 * @brief load bundle configuration and construct dependences
 */
class BundleBoxLoader {
public:
    BundleBoxLoader();
    ~BundleBoxLoader();

    void load(const std::string& root, Resolver& resolver) throw(BundleException);
    bool has(const std::string& symbolicName);
    std::string get(const std::string& symbolicName);

private:
    std::map<std::string, std::string> names;
};

#endif // BUNDLE_BOX_LOADER_H
