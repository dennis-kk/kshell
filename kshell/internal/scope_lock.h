#ifndef SCOPE_LOCK_H
#define SCOPE_LOCK_H

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#else
#   include <pthread.h>
#endif // WIN32

class RecursiveLock {
public:
    RecursiveLock();
    ~RecursiveLock();
    void lock();
    void unlock();

#   ifndef WIN32
    inline pthread_mutex_t* getMutex() {
        return &_mutex;
    }
#   endif // WIN32

private:
#   ifdef WIN32
    CRITICAL_SECTION _cs;
#   else
    pthread_mutex_t _mutex;
#   endif // WIN32
};

template <typename T>
class ScopeLock {
public:
    ScopeLock(T* lock) {
        _lock = lock;
        _lock->lock();
    }

    ~ScopeLock() {
        _lock->unlock();
    }

private:
    T* _lock;
};

#endif // SCOPE_LOCK_H
