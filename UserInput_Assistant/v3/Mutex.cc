#include "Mutex.h"
#include <iostream>
using namespace std;

namespace zwp
{

void Mutex::lock()
{
    if(pthread_mutex_lock(&mutex_t))
    {
        perror("pthread_mutex_lock");
    }; 
    _isLocking=true;
}

void Mutex::unlock()
{
    if(pthread_mutex_unlock(&mutex_t))
    {
        perror("pthread_mutex_unlock");
    };
    _isLocking=false;
}
}



