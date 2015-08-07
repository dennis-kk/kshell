#ifndef CONDITION_H
#define CONDITION_H

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#else
#   include <pthread.h>
#endif // WIN32

#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif // INFINITE

#include "scope_lock.h"

class Condition {
public:
    Condition();
    ~Condition();
    void wait();
    void signal();

private:
    bool          _signal;
    int           _wait;
    RecursiveLock _lock;
#   ifdef WIN32
    HANDLE _event; 
#   else
    pthread_cond_t _cond;
#   endif // WIN32
};

#endif // CONDITION_H
