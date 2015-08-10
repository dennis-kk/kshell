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

#ifndef BUNDLE_CONFIG_H
#define BUNDLE_CONFIG_H

#include "pragma.h"

#include <string>
#include "path.h"

#include "lua.hpp" // Lua

/**
 * @brief simple configuration tools
 */
class BundleConfig {
public:
    BundleConfig() {
        _vm = NULL;
    }

    ~BundleConfig() {
        if (NULL == _vm) {
            return;
        }
        lua_close(_vm);
		_vm = NULL;
    }

    const std::string& getProperty(const std::string& key) {
		static std::string null = "";
		_temp = "";
        if (NULL == _vm) {
            return null;
        }
        lua_getglobal(_vm, key.c_str());
	    if (lua_isstring(_vm, -1)) {
			_temp = lua_tostring(_vm, -1);;
		    return _temp;
	    }
        return null;
    }

    bool open(const std::string& path) {
        if (NULL != _vm) {
            lua_close(_vm);
		    _vm = NULL;
        }
        Path rootPath(path);
        if(!rootPath.isExist()) {
            return false;
        }
        // initialize Lua
        _vm = luaL_newstate();
        luaL_openlibs(_vm);
        if (luaL_dofile(_vm, path.c_str())) {
            lua_close(_vm);
		    _vm = NULL;
            return false;
        }
        return true;
    }

private:
    BundleConfig(const BundleConfig&) {
    }

private:
    lua_State* _vm;   ///< Lua instance
    std::string _path; ///< config file path
	std::string _temp;
};

#endif // BUNDLE_CONFIG_H
