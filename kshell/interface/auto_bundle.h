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
