/*
 * Copyright (c) 2013-2015, dennis wang
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
