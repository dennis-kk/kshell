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

#include "condition.h"

Condition::Condition()
: _wait(0) {
#   ifdef WIN32
    _event = CreateEventA(0, TRUE, FALSE, 0); 
#   else
    pthread_cond_init(&_cond, 0);
#   endif // WIN32
}

Condition::~Condition() {
#   ifdef WIN32
    CloseHandle(_event);
#   else
    pthread_cond_destroy(&_cond);
#   endif // WIN32
}

void Condition::wait() {
#   ifdef WIN32
    ScopeLock<RecursiveLock> guard(&_lock);
    if (_signal) {
        return;
    }
    _wait += 1;
    WaitForSingleObject(_event, INFINITE);
    _wait -= 1;
    if (!_wait) {
        ResetEvent(_event);
        _signal = false;
    }
#   else
    ScopeLock<RecursiveLock> guard(&_lock);
    pthread_cond_wait(&_cond, _lock.getMutex());
#   endif // WIN32
}

void Condition::signal() {
    ScopeLock<RecursiveLock> guard(&_lock);
#   ifdef WIN32
    SetEvent(_event);
#   else
    pthread_cond_broadcast(&_cond);
#   endif // WIN32
    _signal = true;
}
