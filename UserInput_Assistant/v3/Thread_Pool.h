#ifndef  __ZWP_THREAD_POOL_H__
#define  __ZWP_THREAD_POOL_H__
#include "TaskQueue.h"
#include "Thread.h"
#include <memory>
#include <vector>
namespace  zwp
{

class Thread_Pool
{
public:
    using Task=std::function<void()>;
    Thread_Pool(size_t task_num,size_t thread_num)
    :task_num(task_num)
    ,thread_num(thread_num)
    ,_Exit(false)
    {
        //threads.resize(thread_num);
    }
    ~Thread_Pool() {}
    void start();
    void addTask(Task &&);
    Task getTask();
    void stop(); 
    void threadFunc();
private:
    size_t task_num;
    size_t thread_num;
    std::vector<std::unique_ptr<Thread>> threads;
    TaskQueue que;
    bool _Exit;
};

}
#endif
