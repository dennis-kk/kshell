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

#ifndef IGNORE_H
#define IGNORE_H

#include <set>
#include <string>

#include "util.h"
#include "bundle_config.h"

/**
 * @brief ignore bundle list utility
 */
class Ignore {
private:
    typedef std::set<std::string> NameSet;

public:
    Ignore() {
        _ok = false;
    }

    ~Ignore() {
    }

    void open(const std::string& path) {
        _ok = _config.open(path);
        if (!_ok) {
            return;
        }

        std::string list = _config.getProperty("ignore");

        std::vector<std::string> tokens;
        split(list, ";", tokens);
        std::vector<std::string>::iterator token = tokens.begin();
        for (; token != tokens.end(); token++) {
            _nameSet.insert(*token);
        } 
    }

    bool ignore(const std::string& name) {
        if (!_ok) {
            return false;
        }
        return (_nameSet.end() != _nameSet.find(name));
    }

private:
    BundleConfig _config;
    NameSet      _nameSet;
    bool         _ok;
};

#endif // IGNORE_H
