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
