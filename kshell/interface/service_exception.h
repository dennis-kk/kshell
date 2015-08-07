#ifndef SERVICE_EXCEPTION_H
#define SERVICE_EXCEPTION_H

#include "pragma.h"

#include <string>

class ServiceException {
public:
    explicit ServiceException(const std::string& message) {
        _reason = message;
    }

	virtual ~ServiceException() {
	}

    std::string what() {
        return _reason;
    }

private:
    std::string _reason;
};

#endif // SERVICE_EXCEPTION_H
