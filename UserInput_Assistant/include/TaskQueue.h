#ifndef __ZWP_TASKQUEUE_H__
#define __ZWP_TASKQUEUE_H__
#include "Mutex.h"
#include "Condition.h"
#include <queue>
#include <functional>
namespace  zwp
{

using Task=std::function<void()>;
class TaskQueue
{
public:
    TaskQueue(size_t size=10)
    :
    queueSize(size)
    ,que()
    ,notEmpty(_mutex)
    ,notFull(_mutex)
    ,flag(true)
    {}
    ~TaskQueue() {}
    void push(Task &&);
    Task pop();
    bool empty() const ;
    bool full() const ;
    void wake_up();
private:
    size_t queueSize;
    std::queue<Task> que;
    Mutex _mutex;
    Condition notEmpty;
    Condition notFull;
    bool flag;
};

}
#endif
