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

#ifndef BUNDLE_CONTEXT_H
#define BUNDLE_CONTEXT_H

#include "pragma.h"

#include <string>
#include <vector>

#include "bundle_exception.h"
#include "bundle_guid.h"

class Bundle;
class BundleReference;
class FrameworkReference;
class BundleListener;
class ServiceListener;
class Framework;

/**
 * @brief bundle's context
 */
class BundleContext {
public:
    virtual ~BundleContext() {}
    /**
     * @brief add a bundle life-cycle event listener
     */
	virtual void addBundleListener(BundleListener* listener) = 0;

    /**
     * @brief remove a bundle life-cycle event listener
     */
    virtual void removeBundleListener(BundleListener* listener) = 0;

    /**
     * @brief add a service life-cycle event listener
     */
	virtual void addServiceListener(ServiceListener* listener) = 0;

    /**
     * @brief remove a service life-cycle event listener
     */
    virtual void removeServiceListener(ServiceListener* listener) = 0;

    /**
     * @brief get bundle which bundle context holds
     */
	virtual BundleReference* getBundle() = 0;

    /**
     * @brief get other bundle by bundle identifier
     */
	virtual BundleReference* getBundle(BundleGuid id) = 0;

    /**
     * @brief get other bundle by bundle symbolic name
     */
	virtual BundleReference* getBundle(const std::string& symbolicName) = 0;

    /**
     * @brief get all bundle that osgi loaded
     */
	virtual std::vector<BundleReference*> getBundles() = 0;

    /**
     * @brief get bundle's configuration value by key
     */
	virtual const std::string& getProperty(const std::string& key) = 0;

    /**
     * @brief get osgi framework instance(singleton)
     */
    virtual Framework* getFramework() = 0;

    /**
     * @breif get bundle's location
     */
    virtual const std::string& getLocation() = 0;

    /**
     * @brief get executable binary path
     */
    virtual const std::string& getShellLocation() = 0;
};

#endif // BUNDLE_CONTEXT_H
