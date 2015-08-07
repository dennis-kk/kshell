#ifndef BUNDLE_EXCEPTION_H
#define BUNDLE_EXCEPTION_H

#include "pragma.h"

#include <string>

class BundleException {
public:
    explicit BundleException(const char* message) {
        if (NULL != message) {
            _reason = message;
        }
    }

    explicit BundleException(const std::string& message) {
        _reason = message;
    }

    BundleException(const BundleException& rht) {
        _reason = rht._reason;
    }

	~BundleException() throw() {
	}

    std::string what() {
        return _reason;
    }

private:
    std::string _reason;
};

#endif // BUNDLE_EXCEPTION_H
