#include <sstream>
#include <string>

#include "util.h"
#include "path.h"
#include "bundle_box_loader.h"
#include "bundle_config.h"

BundleBoxLoader::BundleBoxLoader() {
}

BundleBoxLoader::~BundleBoxLoader() {
}

void BundleBoxLoader::load(const std::string& root, Resolver& resolver) throw(BundleException) {
    Path rootPath(root);
    if (!rootPath.isExist()) {
        std::stringstream reason;
        reason << "Bundle path [" << root << "]" << " is not exists";
        throw BundleException(reason.str());
    }

    std::vector<std::string> directories;
    if (!rootPath.childPath(directories)) {
        std::stringstream reason;
        reason << "Bundle path [" << root << "]" << " iterate failed";
        throw BundleException(reason.str());
    }

    std::vector<std::string>::iterator iterator = directories.begin();
    for (; iterator != directories.end(); iterator++) {
    #   ifdef WIN32
        std::string configPath = *iterator + "\\meta.inf";
    #   else
        std::string configPath = *iterator + "/meta.inf";
    #   endif

        BundleConfig config;

        // open configuration
        if (!config.open(configPath)) {
            std::stringstream reason;
            reason << "Bundle meta file [" << configPath << "]" << " is not exists";
            throw BundleException(reason.str());
        }

        std::string symbolicName = config.getProperty("symbolicName");

        if (symbolicName.empty()) {
            std::stringstream reason;
            reason << "Need a 'symbolicName' in [" << configPath << "]";
            throw BundleException(reason.str());
        }

        std::string exports = config.getProperty("require");

        if (!exports.empty()) {
            std::vector<std::string> tokens;
            split(exports, ";", tokens);
            std::vector<std::string>::iterator token = tokens.begin();
            for (; token != tokens.end(); token++) {  
                resolver.addDependence(symbolicName, *token);
            }  
        } else {
            resolver.addDependence(symbolicName, "");
        }

        names.insert(std::make_pair(symbolicName, *iterator));
    }
}

bool BundleBoxLoader::has(const std::string& symbolicName) {
    return (names.end() != names.find(symbolicName));
}

std::string BundleBoxLoader::get(const std::string& symbolicName) {
    return names.find(symbolicName)->second;
}
