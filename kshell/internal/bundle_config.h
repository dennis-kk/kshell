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
