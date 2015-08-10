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
