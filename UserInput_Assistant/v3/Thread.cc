#include "Thread.h"
#include "LRUCache.h"
#include "Mutex.h"
#include <iostream>
#include <errno.h>
using namespace std;


__thread LRUCache *name=nullptr;
__thread zwp::Mutex *_mutex=nullptr;
extern __thread std::vector<std::pair<LRUCache*,zwp::Mutex*>> *LRUManager;
namespace zwp
{

std::vector<std::pair<LRUCache*,Mutex*>> ** Thread::LRUManagerPtr=&LRUManager;
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
   if(!name&&!_mutex)
   {
      name=new LRUCache(10);
      _mutex=new Mutex();
   }
   //(*LRUManagerPtr)->push_back(make_pair(name,&_mutex));
   (*LRUManagerPtr)->push_back(make_pair(name,_mutex));
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
