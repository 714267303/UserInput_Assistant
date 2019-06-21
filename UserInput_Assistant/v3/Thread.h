#pragma once
#include <pthread.h>
#include <functional>

namespace zwp
{

class Thread
{
public:
    using function_cb=std::function<void()>;
    Thread(function_cb &&cb) 
    :pid(0)
     ,isRunning(false)
     ,_cb(std::move(cb))
    {}
    ~Thread();
    void start();
    void join();
private:
    static void* threadFunc(void *);
private:
    pthread_t pid;
    bool isRunning;
    function_cb _cb;
};


}
