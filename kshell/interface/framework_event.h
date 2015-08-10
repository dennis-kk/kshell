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

#ifndef FRAMEWORK_EVENT_H
#define FRAMEWORK_EVENT_H

class Framework;

class FrameworkEvent {
public:
    FrameworkEvent(int type, Framework* framework) {
        _type = type;
        _framework = framework;
    }

    FrameworkEvent(const FrameworkEvent& frameworkEvent) {
        operator=(frameworkEvent);
    }

    const FrameworkEvent& operator=(const FrameworkEvent& frameworkEvent) {
        _type = frameworkEvent._type;
        _framework = frameworkEvent._framework;
        return *this;
    }

    int type() {
        return _type;
    }

    Framework* getFramework() {
        return _framework;
    }

private:
     FrameworkEvent() {
        _type = 0;
        _framework = 0;
    }

private:
    int _type;
    Framework* _framework;
};

#endif // FRAMEWORK_EVENT_H
