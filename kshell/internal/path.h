#ifndef PATH_H
#define PATH_H

#include <vector>
#include <string>

class Path {
public:
    Path(const std::string& path);
    ~Path();
    bool isExist();
    bool childPath(std::vector<std::string>& children);

private:
    std::string _path;
};

#endif // PATH_H
