#ifndef  __ZWP_CONDITION_H__
#define  __ZWP_CONDITION_H__
#include "Mutex.h"
#include <pthread.h>
#include <errno.h>
namespace zwp
{

class Condition
{
public:
    Condition(Mutex &m)
    :_mutex(m)
    {
        if(pthread_cond_init(&cond_t,NULL))    
        {
            perror("pthread_cond_init");
        }
    }
    ~Condition()
    {
        if(pthread_cond_destroy(&cond_t))
        {
            perror("pthread_cond_destroy");
        }
    }
    void wait()
    {
        if(pthread_cond_wait(&cond_t,_mutex.getMutexLockPtr()))
        {
            perror("pthread_cond_wait");
        }

    }
    void notify()
    {
        if(pthread_cond_signal(&cond_t))
        {
            perror("pthread_cond_signal");
        }
    }
    void notify_all()
    {
        if(pthread_cond_broadcast(&cond_t))
        {
            perror("pthread_cond_broadcast");
        }
    }
private:
    pthread_cond_t cond_t;
    Mutex &_mutex;
};

}
#endif
