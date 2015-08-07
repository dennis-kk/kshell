#include "scope_lock.h"

RecursiveLock::RecursiveLock() {
#   ifdef WIN32
    // recusive in nature
    InitializeCriticalSection(&_cs);        
#   else
    // recursive lock
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&_mutex, &attr);
#   endif // WIN32
}

RecursiveLock::~RecursiveLock() {
#   ifdef WIN32
    DeleteCriticalSection(&_cs);
#   else
    pthread_mutex_destroy(&_mutex);
#   endif // WIN32
}

void RecursiveLock::lock() {
#   ifdef WIN32
    EnterCriticalSection(&_cs);
#   else
    pthread_mutex_lock(&_mutex);
#   endif // WIN32
}

void RecursiveLock::unlock() {
#   ifdef WIN32
    LeaveCriticalSection(&_cs);
#   else
    pthread_mutex_unlock(&_mutex);
#   endif // WIN32
}
