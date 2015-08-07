#include "pragma.h"

#include "misc.h"

#ifndef WIN32
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#endif // WIN32

std::string getExePath() {
    const static int SIZE = 512;
    char path[SIZE] = {0};
#ifdef WIN32    
    ::GetModuleFileNameA(NULL, path, sizeof(path));
#else
    int result = readlink("/proc/self/exe", path, sizeof(path));
    if (result < 0 || (result >= SIZE - 1)) {
        return "";
    }
    path[result] = '\0';
    for (int i = result; i >= 0; i--) {
        if (path[i] == '/') {
            path[i] = '\0';
            break;
        }
    }
    return path;
#endif // WIN32
    std::string temp(path);
    size_t index = temp.rfind("\\");
    std::string execPath;
    execPath.append(temp, 0, index);
    return execPath;
}
