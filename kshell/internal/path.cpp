#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   include "shlwapi.h"
#   pragma comment(lib, "shlwapi.lib")
#else
#   include <stdio.h>  
#   include <string.h> 
#   include <stdlib.h>  
#   include <dirent.h>  
#   include <sys/stat.h>  
#   include <unistd.h>  
#   include <sys/types.h> 
#endif // WIN32

#include "path.h"

Path::Path(const std::string& path) {
    _path = path;
#ifdef WIN32
    if (path.at(path.size() - 1) != '\\') {
        _path += '\\';
    }
#else
    if (path.at(path.size() - 1) != '/') {
        _path += '/';
    }
#endif // WIN32
}

Path::~Path() {
}

bool Path::isExist() {
#ifdef WIN32
    return (TRUE == PathFileExistsA(_path.c_str()));
#else
    return !access(_path.c_str(), F_OK);
#endif // WIN32
}

bool Path::childPath(std::vector<std::string>& children) {
    if (!isExist()) {
        return false;
    }
#ifdef WIN32
    WIN32_FIND_DATAA findData;
    HANDLE           handle = INVALID_HANDLE_VALUE;
    handle = FindFirstFileA(_path.c_str(), &findData);
    if (handle == INVALID_HANDLE_VALUE) {
        return false;
    }
    while (FindNextFileA(handle, &findData)) {
        if (strcmp(findData.cFileName, ".") == 0 ||
            strcmp(findData.cFileName, "..") == 0 ){
            continue;
        }
        std::string path = _path + findData.cFileName;
        children.push_back(path);
    }
#else
    DIR           *dir = 0;  
    struct dirent *ent = 0;
    dir = opendir(_path.c_str());
    if (!dir) {
        return false;
    }
    while (0 != (ent = readdir(dir))) {
        if (ent->d_type & DT_DIR) {
            if (strcmp(ent->d_name,".") == 0 ||
                strcmp(ent->d_name,"..") == 0) {
                    continue;
            }
            std::string path = _path + ent->d_name;
            children.push_back(path);
        }
    }
#endif // WIN32
    return true;
}