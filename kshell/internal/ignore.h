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
