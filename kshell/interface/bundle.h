#ifndef BUNDLE_H
#define BUNDLE_H


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

#include "pragma.h"

#include "bundle_guid.h"
#include "bundle_exception.h"
#include "class_not_found_exception.h"

class BundleContext;
class Service;
class ServiceReference;

/**
 * @brief Bundle - service container
 */
class Bundle {
public:
	static const int UNINSTALLED = 1; ///< Bundle was de-installed - reserved for future
	static const int INSTALLED = 2;   ///< Bundle was installed
	static const int RESOLVED = 4;    ///< Bundle was resolved - reserved for future
	static const int STARTING = 8;    ///< Bundle was starting
	static const int STOPPING = 16;   ///< Bundle was stopping
	static const int ACTIVE = 32;     ///< Bundle was actived
    static const int STOPPED = 64;    ///< Bundle was stopped

public:
    virtual ~Bundle() {}
    /**
     * @brief get BundleContext of current bundle
     */
    virtual BundleContext* getBundleContext() = 0;

    /**
     * @brief get Bundle's GUID
     */
	virtual BundleGuid getBundleId() = 0;

    /**
     * @brief get Bundle's version string
     */
    virtual const std::string& getVersion() = 0;

    /**
     * @brief get Bundle's location
     */
	virtual const std::string& getLocation() = 0;

    /**
     * @brief get Bundle's state
     */
	virtual int state() = 0;

    /**
     * @brief get Bundle's symbolic name
     */
	virtual const std::string& getSymbolicName() = 0;

    /**
     * @brief create a new service from bundle
     * @param name service name
     * @return ServiceReference
     */
	virtual ServiceReference* loadClass(const std::string& name) throw(ClassNotFoundException) = 0;

    /**
     * @brief start a bundle
     */
	virtual void start() throw(BundleException) = 0;

    /**
     * @brief stop a bundle
     */
	virtual void stop() throw(BundleException) = 0;
};

#endif // BUNDLE_H
