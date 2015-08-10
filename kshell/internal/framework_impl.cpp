#include <iostream>
#include <ctime>

#include "path.h"
#include "framework_impl.h"
#include "pid_file.h"
#include "misc.h"

FrameworkImpl::FrameworkImpl() {
    _box      = NULL;
    _flag     = false;
    _location = "";
}

FrameworkImpl::~FrameworkImpl() {
    _box        = NULL;
    _location   = "";
}

void FrameworkImpl::start() throw(FrameworkException) {
    do {
        // startup
        ScopeLock<RecursiveLock> guard(&_lock);
        _box = new BundleBox();
        try {
            _box->start();
        } catch (BundleException& exc) {
            throw FrameworkException(exc.what());
        }
    } while (0);

    do {
        // write PID file
        PidFile pidFile;
    } while (0);

    _location = getExePath();
    _flag     = true;

    bool result = false;
    while (!result) {
        // get stop result
        result = _box->waitForStop();
        if (result) {
            break;
        }
    }

    do {
        // cleanup
        if (NULL != _box) {
            delete _box;
        }
        _box = NULL;
    } while (0);
}

void FrameworkImpl::stop() throw(FrameworkException) {
    if (!_flag) {
        throw FrameworkException("Framework starting phase was not finished");
    }
    if (NULL != _box) {
        try {
            _box->stop();
        } catch (BundleException exc) {
            throw FrameworkException(exc.what());
        }
    }
}

const std::string& FrameworkImpl::getLocation() {
    return _location;
}
