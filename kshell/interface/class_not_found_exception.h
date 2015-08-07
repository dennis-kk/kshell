#ifndef CLASS_NOT_FOUND_EXCEPTION_H
#define CLASS_NOT_FOUND_EXCEPTION_H

#include "pragma.h"

#include <string>

class ClassNotFoundException {
public:
    explicit ClassNotFoundException(const char* message) {
        if (NULL != message) {
            _reason = message;
        }
    }

    explicit ClassNotFoundException(const std::string& message) {
        _reason = message;
    }

    ClassNotFoundException(const ClassNotFoundException& rht) {
        _reason = rht._reason;
    }

	~ClassNotFoundException() throw() {
	}

    std::string what() {
        return _reason;
    }

private:
    std::string _reason;
};

#endif // CLASS_NOT_FOUND_EXCEPTION_H
