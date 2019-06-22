#include "../include/TcpServer.h"
#include "../include/Thread_Pool.h"
#include "../include/Task.h"
#include "../include/Timer.h"
#include "../include/LRUCache.h"

#include "../include/Configuration.h"
#include "../include/DictProducer.h"
#include "../include/IndexProducer.h"
#include "../include/SplitToolCppJieba.h"
#include <iostream>
#include <sstream>
using namespace std;
using namespace zwp;
using TcpConnectionPtr=std::shared_ptr<TcpConnection>;

void Init(int &threadNum,int &taskNum,int &port,string &ip);
void connectFunc(const TcpConnectionPtr &);
void recvmsgFunc(const TcpConnectionPtr &);
void disconnectFunc(const TcpConnectionPtr &);
void sendmsgFunc(const TcpConnectionPtr &);

extern __thread zwp::LRUCache *name;
__thread vector<pair<zwp::LRUCache*,Mutex*>> *LRUManager=nullptr;

class LRU_clock
{

public:
    void process(vector<pair<zwp::LRUCache*,Mutex*>> *LRUManager)
    {
#if 1
        int i=0;
        for(auto &e:*LRUManager)
        {
            auto cache_data=e.first;
            cout<<i++<<" cache"<<":"<<endl;
            for(auto i:*(cache_data->getUpdateList()))
            {
                cout<<"     "<<i.key<<":"<<i.value<<endl;
            };
            cout<<endl;
        }
#endif
        for(auto &e:*LRUManager)
        {
            //cout<<"LRUCache:"<<e.first<<"-----"<<"Mutex:"<<e.second<<endl;
            //更新缓存
            auto lock=e.second;
            auto Cache=e.first;
            if(lock==zwp::_mutex)
                continue;
            lock->lock();
            list<zwp::LRUCache::Point> temp(*Cache->getUpdateList());
            lock->unlock();

            auto it=temp.begin();
            for(int i=0;i<Cache->getUpdateNum()&&it!=temp.end();i++,it++)
            {
                zwp::name->put(it->key,it->value);
            }
        }

        for(auto &e:*LRUManager)
        {
            //更新所有缓存
            auto lock=e.second;
            auto Cache=e.first;
            auto list2update=zwp::name->getUpdateList();
            if(lock==zwp::_mutex)
                continue;
            lock->lock();
            for(auto it=list2update->begin();it!=list2update->end();it++)
            {
                Cache->put(it->key,it->value);
            }
            lock->unlock();
        }
    }
};

Thread_Pool *pool;
int main()
{
    LRUManager=new vector<pair<zwp::LRUCache*,zwp::Mutex*>>;
    Configuration *config=Configuration::getInstance("../conf/config");
    SplitToolCppJieba cppjieba;

    DictProducer dic(config->getConfigMap().find("STORE")->second,&cppjieba);
    dic.build_dict();

    IndexProducer::getInstance(config->getConfigMap().find("STORE")->second,
                               config->getConfigMap().find("INDEX")->second);

    zwp::Thread p=zwp::Thread(bind(
                                   &zwp::Timer::start,
                                   zwp::Timer(
                                              3,
                                              6,
                                              bind(&LRU_clock::process,LRU_clock(),LRUManager)
                                             )
                                  ));

    p.start();
    string ip;
    int threadNum,taskNum,port;
    Init(threadNum,taskNum,port,ip);

    Thread_Pool thread_pool(taskNum,threadNum);
    pool=&thread_pool;
    pool->start();

    TcpServer server(ip,port,
                     connectFunc,
                     recvmsgFunc,
                     disconnectFunc,
                     sendmsgFunc
                    );
    server.start();

    return 0;
}


























void connectFunc(const TcpConnectionPtr &tcp)
{
    cout<<"connecting..."<<endl;
    cout<<*tcp<<endl;
};

void recvmsgFunc(const TcpConnectionPtr &tcp)
{
    string s=tcp->recv();
    MyTask task(s,tcp);
    pool->addTask(bind(&zwp::MyTask::process,task));
};

void disconnectFunc(const TcpConnectionPtr &tcp)
{
    cout<<*tcp<<"----disconnect"<<endl;
};

void sendmsgFunc(const TcpConnectionPtr &tcp)
{
    tcp->send("hello");
};

void Init(int &threadNum,int &taskNum,int &port,string &ip)
{
    zwp::Configuration *p=zwp::Configuration::getInstance("../conf/config");
    auto ConfigMap=p->getConfigMap();
    {
        stringstream s(ConfigMap.find("THREADNUM")->second);
        s>>threadNum;
    }
    {
        stringstream s(ConfigMap.find("TASKNUM")->second);
        s>>taskNum;
    }
    {
        stringstream s(ConfigMap.find("PORT")->second);
        s>>port;
    }
    {
        stringstream s(ConfigMap.find("IP")->second);
        s>>ip;
    }
}
#if 0
Accepter accepter("192.168.19.128",8888);
accepter.ready();
TcpConnection connection(accepter.accept());
connection.send("hello word");
cout<<connection.recv()<<endl;
cout<<connection.recv()<<endl;
cout<<connection.recv()<<endl;
cout<<connection<<endl;
int fd=open("main.cc",O_EXCL,0666);
if(-1==fd)
{
    perror("open");
    return 1;
}
SocketIO s(0);
char buf[10]={0};
s.readn(buf,sizeof(buf));
cout<<"case 1:\n"
<<buf<<endl;

#if 1
int ret=-1;
s.read32int(ret);
cout<<"case 2:\n"
<<ret<<endl;

s.readline(buf,sizeof(buf));
cout<<"case 3:\n"
<<buf<<endl;
#endif
#endif

