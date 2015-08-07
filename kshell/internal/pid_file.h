#ifndef PID_FILE_H
#define PID_FILE_H

#include <string>

class PidFile {
public:
#   ifndef WIN32
    PidFile(const std::string& path);
    void kill();
#   endif // WIN32
    PidFile();
    ~PidFile();

private:
#   ifndef WIN32
    int _pid;
#   endif // WIN32
};

#endif // PID_FILE_H
