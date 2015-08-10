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

#include "bundle_loader.h"

BundleLoader::BundleLoader() {
    _handle = 0;
}

BundleLoader::~BundleLoader() {
    if (isValid()) {
        unload();
    }
}

void BundleLoader::load(const std::string& path, std::string name) throw(BundleException) {
    ScopeLock<RecursiveLock> guard(&_lock);
	std::stringstream FullPath;
	FullPath << path;
#	ifdef WIN32
	    FullPath << "\\" << name;	
#	else
	    FullPath << "/" << name;
#   endif  // WIN32
#	ifdef WIN32
        SetDllDirectoryA(path.c_str());
        _handle = ::LoadLibraryA(FullPath.str().c_str());
#	else
		_handle = ::dlopen(FullPath.str().c_str(), RTLD_NOW);
#	endif // WIN32
}

void BundleLoader::unload() throw(BundleException) {
    ScopeLock<RecursiveLock> guard(&_lock);
#	ifdef WIN32
		::FreeLibrary(_handle); // free plugin manager plugin
#	else
		dlclose(_handle); // free plugin manager plugin
#	endif // WIN32
    _handle = 0;
}

Service* BundleLoader::newClass(const std::string& entry, std::string name) throw(ClassNotFoundException) {
    ScopeLock<RecursiveLock> guard(&_lock);
#   ifdef WIN32
    NewServiceFunc func = (NewServiceFunc)::GetProcAddress(_handle, entry.c_str());
#   else
    NewServiceFunc func = (NewServiceFunc)::dlsym(_handle, entry.c_str());
#   endif
    if (NULL == func) {
        throw ClassNotFoundException("Cannot find service entry");
    }
    void* object = func(name.c_str());
    if (NULL == object) {
        std::stringstream reason;
        reason << "new class ["<< name << "]" << "failed";
        throw ClassNotFoundException(reason.str());
    }
    return reinterpret_cast<Service*>(object);
}

void BundleLoader::destroyClass(const std::string& entry, Service* service, std::string name) {
    ScopeLock<RecursiveLock> guard(&_lock);
#   ifdef WIN32
    DestroyServiceFunc func = (DestroyServiceFunc)::GetProcAddress(_handle, entry.c_str());
#   else
    DestroyServiceFunc func = (DestroyServiceFunc)::dlsym(_handle, entry.c_str());
#   endif
    if (NULL == func) {
        throw ClassNotFoundException("Cannot find service entry");
    }
    func(service, name.c_str());
}

BundleActivator* BundleLoader::newActivator(const std::string& entry) throw(ClassNotFoundException) {
    ScopeLock<RecursiveLock> guard(&_lock);
#   ifdef WIN32
    NewActivatorFunc func = (NewActivatorFunc)::GetProcAddress(_handle, entry.c_str());
#   else
    NewActivatorFunc func = (NewActivatorFunc)::dlsym(_handle, entry.c_str());
#   endif
    if (NULL == func) {
        throw ClassNotFoundException("Cannot find activator entry");
    }
    void* object = func();
    if (NULL == object) {
        std::stringstream reason;
        reason << "new class [activator] failed";
        throw ClassNotFoundException(reason.str());
    }
    return reinterpret_cast<BundleActivator*>(object);
}

bool BundleLoader::isValid() {
    ScopeLock<RecursiveLock> guard(&_lock);
	return (_handle ? true : false);
}
