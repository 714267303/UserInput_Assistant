#include "../include/Thread.h"
#include "../include/Mutex.h"
#include "../include/LRUCache.h"
#include "../include/Configuration.h"
#include <iostream>
#include <errno.h>
using namespace std;

extern __thread std::vector<std::pair<zwp::LRUCache*,zwp::Mutex*>> *LRUManager;

namespace zwp{
 __thread LRUCache *name;
__thread zwp::Mutex *_mutex=nullptr;
}
namespace zwp
{

std::vector<std::pair<LRUCache*,Mutex*>> ** Thread::LRUManagerPtr=&LRUManager;
void Thread::start()
{
   LRUManagerPtr=&LRUManager;
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
   if(!name&&!_mutex)
   {
       name=new LRUCache(atoi(Configuration::getInstance("")->getConfigMap()
                         .find("CACHESIZE")->second.c_str()));
       _mutex=new Mutex();
       (*LRUManagerPtr)->push_back(make_pair(name,_mutex));
   }
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
