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
