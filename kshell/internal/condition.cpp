#include "condition.h"

Condition::Condition()
: _wait(0) {
#   ifdef WIN32
    _event = CreateEventA(0, TRUE, FALSE, 0); 
#   else
    pthread_cond_init(&_cond, 0);
#   endif // WIN32
}

Condition::~Condition() {
#   ifdef WIN32
    CloseHandle(_event);
#   else
    pthread_cond_destroy(&_cond);
#   endif // WIN32
}

void Condition::wait() {
    ScopeLock<RecursiveLock> guard(&_lock);
    if (_signal) {
        return;
    }
    _wait += 1;
#   ifdef WIN32
    WaitForSingleObject(_event, INFINITE);
    _wait -= 1;
    if (!_wait) {
        ResetEvent(_event);
        _signal = false;
    }
#   else
    pthread_cond_wait(&_cond, _lock.getMutex());
#   endif // WIN32
}

void Condition::signal() {
    ScopeLock<RecursiveLock> guard(&_lock);
#   ifdef WIN32
    SetEvent(_event);
#   else
    pthread_cond_broadcast(&_cond);
#   endif // WIN32
    _signal = true;
}
