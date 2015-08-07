#ifndef AUTO_BUNDLE_H
#define AUTO_BUNDLE_H

#include "bundle_reference.h"
#include "bundle_exception.h"

/**
 * @brief helper class for accessing BundleReference
 */
class AutoBundle {
public:
    /**
     * ctor
     */
    AutoBundle() {
        _reference = NULL;
        _bundle = NULL;
    }

    /**
     * ctor
     */
    AutoBundle(BundleReference* reference) {
        _bundle    = NULL;
        _reference = reference;
        if (NULL != _reference) {
            _bundle = _reference->get();
        }
    }

    /**
     * copy ctor
     */
    AutoBundle(const AutoBundle& rht) {
        operator=(rht);
    }

    /**
     * assignment
     */
    const AutoBundle& operator=(const AutoBundle& rht) {
        _bundle    = NULL;
        _reference = rht._reference;
        if (NULL != _reference) {
            _bundle = _reference->get();
        }
		return *this;
    }

    /**
     * dtor
     */
    ~AutoBundle() {
        if (NULL == _reference) {
            return;
        }
        // increase reference count accordinglly
        _reference->unget();
    }

    /**
     * test whether the reference is valid
     */
    bool isOk() {
        if (NULL == _reference) {
            return false;
        }
        return _reference->isOk();
    }

    /**
     * get bundle instance and increase reference count automatically
     */
    Bundle* operator->() {
        if (NULL == _reference) {
            throw BundleException("Invalid AutoBundle");
        }
        return _bundle;
    }

	/**
	 * vanish
	 */
	void destroy() {
		if (NULL == _reference) {
            return;
        }
        // increase reference count accordinglly
        _reference->unget();
		_reference = NULL;
	}

private:
    BundleReference* _reference; // BundleReference instance
    Bundle*          _bundle;    // bundle
};

#endif // AUTO_BUNDLE_H
