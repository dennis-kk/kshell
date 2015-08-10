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
#   ifdef WIN32
    WIN32_FIND_DATAA findData;
    HANDLE           handle = INVALID_HANDLE_VALUE;
    std::string      path   = _path + "\\*.*";
    handle = FindFirstFile(path.c_str(), &findData);
    if (handle == INVALID_HANDLE_VALUE) {
        return false;
    }
    _path += '\\';
    for (;;) {
        if (strcmp(findData.cFileName, ".") == 0 ||
            strcmp(findData.cFileName, "..") == 0 ){
            if (!FindNextFile(handle, &findData)) {
                break;
            }
            continue;
        }
        std::string path = _path + findData.cFileName;
        children.push_back(path);
        if (!FindNextFile(handle, &findData)) {
            break;
        }
    }
#   else
    DIR           *dir = 0;  
    struct dirent *ent = 0;
    dir = opendir(_path.c_str());
    if (!dir) {
        return false;
    }
    _path += '/';
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
#   endif // WIN32
    return true;
}

bool Path::remove() {
#   ifdef WIN32
    DeleteFile(_path.c_str());
#   else
    unlink(_path.c_str());
#endif // WIN32
    return true;
}
