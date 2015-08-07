#ifndef INTERRUPTED_EXCEPTION_H
#define INTERRUPTED_EXCEPTION_H

#include "pragma.h"

#include <string>

class InterruptedException {
public:
    explicit InterruptedException(const char* message) {
        if (NULL != message) {
            _reason = message;
        }
    }

    explicit InterruptedException(const std::string& message) {
        _reason = message;
    }

    InterruptedException(const InterruptedException& rht) {
        _reason = rht._reason;
    }

	~InterruptedException() throw() {
	}

    std::string what() {
        return _reason;
    }

private:
    std::string _reason;
};

#endif // INTERRUPTED_EXCEPTION_H
