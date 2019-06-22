#pragma once
#include "Mutex.h"
#include <pthread.h>
#include <functional>
#include <vector>
class LRUCache;

extern __thread LRUCache *name;
extern __thread std::vector<std::pair<LRUCache*,zwp::Mutex*>> *LRUManager;
extern __thread zwp::Mutex *_mutex;

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
    static std::vector<std::pair<LRUCache*,Mutex*>> **LRUManagerPtr;
};


}
