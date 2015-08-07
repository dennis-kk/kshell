#ifndef FRAMEWORK_EXCEPTION_H
#define FRAMEWORK_EXCEPTION_H

#include "pragma.h"

#include <string>

class FrameworkException {
public:
    explicit FrameworkException(const char* message) {
        if (NULL != message) {
            _reason = message;
        }
    }

    explicit FrameworkException(const std::string& message) {
        _reason = message;
    }

    FrameworkException(const FrameworkException& rht) {
        _reason = rht._reason;
    }

	~FrameworkException() throw() {
	}

    std::string what() {
        return _reason;
    }

private:
    std::string _reason;
};

#endif // FRAMEWORK_EXCEPTION_H
