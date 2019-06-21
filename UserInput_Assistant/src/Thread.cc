#include "../include/Thread.h"
#include <iostream>
#include <errno.h>
using namespace std;


namespace zwp
{

void Thread::start()
{
   if(pthread_create(&pid,nullptr,threadFunc,this))
   {
       perror("thread_create");
       return ;
   }
    isRunning=true;
}
void *Thread::threadFunc(void *arg)
{
  //  cout<<"Thread::threadFunc"<<endl;
   Thread * m_thread=static_cast<Thread*>(arg);
   if(m_thread)
       m_thread->_cb();
    return NULL;
}
void Thread::join()
{
    if(isRunning)
    {
        pthread_join(pid,NULL);
        isRunning=false;
    }
}

Thread::~Thread()
{
    if(isRunning)
    {
        pthread_detach(pid);
        isRunning=false;
    }
//    cout<<"~Thread()"<<endl;
}

}
