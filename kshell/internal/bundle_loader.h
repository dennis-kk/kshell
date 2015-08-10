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

#ifndef BUNDLE_LOADER_H
#define BUNDLE_LOADER_H

#include "pragma.h"

#include "scope_lock.h"
#include "bundle.h"
#include "bundle_handle.h"

class Service;
class BundleActivator;

class BundleLoader {
public:
	/**
	 * ctor
	 */
	BundleLoader();

	/**
	 * dtor
	 */
	~BundleLoader();

	/**
	 * load a bundle
	 *
	 * @param path the directory which contais the class
	 * @param name bundle name
	 * @return bundle handle
	 */
    void load(const std::string& path, std::string name) throw(BundleException);

	/**
	 * free a bundle
	 */
	void unload() throw(BundleException);

    /**
     * fork a Service by service name
     */
    Service* newClass(const std::string& entry, std::string name) throw(ClassNotFoundException);

    /**
     * destroy a Service by service name
     */
    void destroyClass(const std::string& entry, Service* service, std::string name);

    /**
     * fork a BundleActivator
     */
    BundleActivator* newActivator(const std::string& entry) throw(ClassNotFoundException);

	/**
	 * test whether the handle is valid
	 *
	 * @param handle bundle handle
	 * @return true on yes, false on no
	 */
	bool isValid();

private:
    OsHandle _handle;
    RecursiveLock _lock;
};

#endif // BUNDLE_LOADER_H
