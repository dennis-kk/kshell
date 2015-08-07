#ifndef AUTO_SERVICE_H
#define AUTO_SERVICE_H

#include "service_reference.h"
#include "service_exception.h"
#include "bundle_context.h"
#include "bundle.h"
#include "auto_bundle.h"

/**
 * @brief helper class for accessing ServiceReference
 */
class AutoServiceReference {
public:
    /**
     * ctor
     */
    AutoServiceReference() {
        _reference = NULL;
    }

    /**
     * copy ctor
     */
    AutoServiceReference(const AutoServiceReference& rht) {
        _reference = NULL;
        operator=(rht);
    }

    /**
     * ctor
     */
    AutoServiceReference(ServiceReference* reference) {
        _reference = reference;
        if (NULL != _reference) {
            _reference->get();
        }
    }

    /**
     * dtor
     */
    ~AutoServiceReference() {
        if (NULL == _reference) {
            return;
        }
        _reference->unget();
    }

    /**
     * assignment
     */
    const AutoServiceReference& operator=(const AutoServiceReference& rht) {
		if (NULL != _reference) {
			_reference->unget();
		}
        _reference = rht._reference;
        if (NULL != _reference) {
            _reference->get();
        }
        return *this;
    }

    /**
     * test whether service reference is valid
     */
    bool isOk() {
        if (NULL == _reference) {
            return false;
        }
        return _reference->isOk();
    }

    /**
     * get service instance and donot change reference count
     */
    ServiceReference* get() const throw(ServiceException) {
        if (NULL == _reference) {
            throw ServiceException("Invalid ServiceReference");
        }
        return _reference;
    }

    /**
     * destroy service instance, the AutoService MUST not access from now on 
     */
    void destroy() {
        if (NULL != _reference) {
            _reference->unget();
            _reference->destroy();
            _reference = NULL;
        }
    }

private:
    ServiceReference*  _reference; // ServiceReference instance
};

/**
 * @brief helper class for accessing Service
 */
template<typename T>
class AutoService {
public:
    /**
     * ctor
     */
    AutoService() {
        _reference = NULL;
        _service = NULL;
    }

    /**
     * copy ctor
     */
    AutoService(const AutoService<T>& rht) {
        _reference = NULL;
        _service = NULL;
        operator=(rht);
    }

    /**
     * ctor
     */
    AutoService(ServiceReference* reference) {
        _service   = NULL;
        _reference = reference;
        if (NULL != _reference) {
            _service = _reference->get();
        }
    }

	/**
     * ctor
     */
    AutoService(const AutoServiceReference& asr) {
        _service   = NULL;
		_reference = asr.get();
        if (NULL != _reference) {
            _service = _reference->get();
        }
    }

    /**
     * dtor
     */
    ~AutoService() {
        if (NULL == _reference) {
            return;
        }
        _reference->unget();
    }

    /**
     * assignment
     */
    const AutoService& operator=(const AutoService<T>& rht) {
		if (NULL != _reference) {
			_reference->unget();
		}
        _reference = rht._reference;
        _service = NULL;
        if (NULL != _reference) {
            _service = _reference->get();
        }
        return *this;
    }

	/**
     * assignment
     */
	const AutoService& operator=(ServiceReference* reference) {
		if (NULL != _reference) {
			_reference->unget();
		}
		_reference = reference;
        _service = NULL;
        if (NULL != _reference) {
            _service = _reference->get();
        }
        return *this;
    }

	/**
     * assignment
     */
	const AutoService& operator=(const AutoServiceReference& asr) {
		if (NULL != _reference) {
			_reference->unget();
		}
		_reference = asr.get();
        _service = NULL;
        if (NULL != _reference) {
            _service = _reference->get();
        }
        return *this;
    }

    /**
     * test whether service reference is valid
     */
    bool isOk() {
        if (NULL == _reference) {
            return false;
        }
        return _reference->isOk();
    }

    /**
     * get service instance and donot change reference count
     */
    T* operator->() throw(ServiceException) {
        if (NULL == _reference || NULL == _service) {
            throw ServiceException("Invalid ServiceReference");
        }
        return dynamic_cast<T*>(_service);
    }

    /**
     * get service instance and donot change reference count
     */
    T* get() throw(ServiceException) {
        if (NULL == _reference || NULL == _service) {
            throw ServiceException("Invalid ServiceReference");
        }
        return dynamic_cast<T*>(_service);
    }

    /**
     * type-cast operator
     */
    operator T*() {
        return dynamic_cast<T*>(_service);
    }

    /**
     * type-cast operator for 'bool'
     */
    operator bool() {
        return isOk();
    }

    /**
     * destroy service instance, the AutoService MUST not access from now on 
     */
    void destroy() {
        if (NULL != _reference) {
            _reference->unget();
            _reference->destroy();
            _reference = NULL;
            _service = NULL;
        }
    }

    ServiceReference* reference() {
        return _reference;
    }

private:
    ServiceReference*  _reference; // ServiceReference instance
    Service*           _service;   // service
};

template <typename T>
static inline AutoService<T> loadClass(BundleContext* context,
    const std::string& bundle, const std::string& name) {
    AutoBundle yourBundle = context->getBundle(bundle);
    if (!yourBundle.isOk()) {
        std::string error = "bundle ";
        error += bundle;
        error += " not found";
        throw ServiceException(error);
    }
    AutoService<T> service = yourBundle->loadClass(name);
    if (!service.isOk()) {
        std::string error = "service ";
        error += name;
        error += " not found";
        throw ServiceException(error);
    }

    return service;
}

#endif // AUTO_SERVICE_H
