#include "TaskQueue.h"
#include <iostream>
#include <functional>
using namespace std;
namespace zwp
{

bool TaskQueue::empty() const
{
    return que.empty();
}

bool TaskQueue::full() const
{
    return queueSize==que.size();
}

void TaskQueue::wake_up()
{
    flag=false;
    notEmpty.notify_all();
}
void TaskQueue::push(Task&& number)
{
    MutexLockGuard autoLock(_mutex);
    while(full())
    {
        notFull.wait();
    }
    que.push(std::move(number));
    notEmpty.notify();
}

Task TaskQueue::pop() 
{
    MutexLockGuard autoLock(_mutex);
    while(flag&&empty())
    {
        notEmpty.wait();
    }
    if(flag)
    {
        Task num=que.front();
        que.pop();
        notFull.notify();
        return num;
    }
    else
    {
        return Task();
    }
}

}
