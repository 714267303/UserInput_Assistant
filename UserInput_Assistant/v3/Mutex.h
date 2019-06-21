#ifndef  __ZWP_MUTEX_H__
#define  __ZWP_MUTEX_H__
#include <mutex>

namespace  zwp
{

class Mutex
{
public:
    Mutex():_isLocking(false)
    {
        if(pthread_mutex_init(&mutex_t,NULL))
        {
            perror("pthread_mutex_init");
        };
    }
    ~Mutex() 
    {
        if(pthread_mutex_destroy(&mutex_t))
        {
            perror("pthread_mutex_destroy");
        };
    }
    bool isLocking() const {    return _isLocking;  }
    pthread_mutex_t * getMutexLockPtr() {   return &mutex_t;    }
    void lock();
    void unlock();
private:
    pthread_mutex_t mutex_t;
    bool _isLocking;
};

class MutexLockGuard
{
public:
    MutexLockGuard(Mutex &mutex_):mutex_(mutex_)
    {
        mutex_.lock();
    }
    ~MutexLockGuard()
    {
        mutex_.unlock();
    }
private:
    Mutex &mutex_;
};
}

#endif
