#include "../include/Thread_Pool.h"
#include <unistd.h>
#include <iostream>

using namespace std;
namespace zwp
{

void Thread_Pool::start()
{
    for(size_t i=0;i!=thread_num;i++)
    {
        unique_ptr<Thread> uptr(
            new Thread(bind(&Thread_Pool::threadFunc,this))
        );
        threads.push_back(std::move(uptr));
        //cout<<"thread_num="<<thread_num<<endl;
    }
    //cout<<"push_back ok~"<<endl;
    for(auto &e:threads)
    {
        e->start();
//        cout<<"start()"<<endl;
    }
}

void Thread_Pool::threadFunc()
{
    //cout<<"Thread_Pool::threadFunc"<<endl;
    while(!_Exit)
    {
        Task task=getTask();
        if(task)
            task();
 //       cout<<"task()"<<endl;
    }
}

Task Thread_Pool::getTask()
{
    Task t=que.pop();
    return t;
}

void Thread_Pool::addTask(Task&& task)
{
    //cout<<"ok1"<<endl;
    que.push(std::move(task));
    //cout<<"ok2"<<endl;
}

void Thread_Pool::stop()
{
    if(!_Exit)
    {
        while(!que.empty())
        {
            sleep(2);
        }
        _Exit=true;
    
        que.wake_up();
        for(auto &e:threads)
        {
            e->join();
        }
    }
}
}
