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
