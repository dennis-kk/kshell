#ifndef COLOR_H
#define COLOR_H

#ifdef WIN32

inline std::ostream& blue(std::ostream &s) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& red(std::ostream &s) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& green(std::ostream &s) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& yellow(std::ostream &s) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& white(std::ostream &s) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return s;
}

#else

#include <cstdio>

inline static std::ostream& blue(std::ostream &s) {
    printf("\033[1;34m");
    return s;
}

inline static std::ostream& red(std::ostream &s) {
    printf("\033[1;31m");
    return s;
}

inline static std::ostream& green(std::ostream &s) {
    printf("\033[1;32m");
    return s;
}

inline static std::ostream& yellow(std::ostream &s) {
    printf("\033[1;33m");
    return s;
}

inline static std::ostream& white(std::ostream &s) {
    printf("\033[30m");
    return s;
}

#endif // WIN32

#endif // COLOR_H
