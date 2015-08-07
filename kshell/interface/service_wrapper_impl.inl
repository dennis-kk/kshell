virtual void initialize() throw(ServiceException) {
	try {
		T::serviceStart();
    } catch (std::exception& exc) {
        throw ServiceException(exc.what());
	}
}

virtual void finalize() throw(ServiceException) {        
	try {
		T::serviceStop();
	} catch (std::exception& exc) {
        throw ServiceException(exc.what());
	}
}

Service* service() {
	return dynamic_cast<Service*>(this);
}

static T* cast(void* service) {
	try {
		return dynamic_cast<T*>(reinterpret_cast<Service*>(service));
	} catch (std::bad_cast&) {
		return NULL;
	}
}