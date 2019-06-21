#include "../include/TcpServer.h"
#include "../include/Thread_Pool.h"
#include "../include/Task.h"

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

Thread_Pool *pool;
int main()
{
    Configuration *config=Configuration::getInstance("../conf/config");
    SplitToolCppJieba cppjieba;
    DictProducer dic(config->getConfigMap().find("STORE")->second,&cppjieba);
    dic.build_dict();
    IndexProducer *indexPtr=IndexProducer::getInstance(config->getConfigMap().find("STORE")->second,
                                                       config->getConfigMap().find("INDEX")->second);
     

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

