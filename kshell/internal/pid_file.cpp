#include <iostream>

#include <stdio.h>
#include <string.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define getpid GetCurrentProcessId
#else
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#endif // WIN32

#include <sstream>
#include <fstream>

#include "pid_file.h"
#include "misc.h"

#ifndef WIN32

PidFile::PidFile(const std::string& path) {
    _pid = 0;
    // get binary directory
    std::string fileName = getExePath() + "/pid";

    // read PID from file
    std::ifstream file;
    file.open(fileName.c_str(), std::ios::in);
    if (file) {
        std::string pid;
        file >> pid;
        _pid = atoi(pid.c_str());
    }

    // cleanup
    file.close();
}

void PidFile::kill() {
    if (0 >= _pid) {
        return;
    }
    std::cout << "Kill kshell SIGQUIT PID(" << _pid << ") ..." << std::endl;
    ::kill(_pid, SIGQUIT);
}

#endif // WIN32

PidFile::PidFile() {
    // get PID
    std::stringstream pid;
    pid << getpid();

    // get current working directory
    std::string fileName = getExePath() + "/pid";

    // write PID to file
    std::ofstream file;
    file.open(fileName.c_str(), std::ios::out | std::ios::trunc);
    if (file) {
        file << pid.str();
    }

    // cleanup
    file.close();
}

PidFile::~PidFile() {
}
