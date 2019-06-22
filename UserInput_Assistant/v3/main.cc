#include "TcpServer.h"
#include "Thread_Pool.h"
#include "Task.h"
#include "Mutex.h"
#include <iostream>
using namespace std;
using namespace zwp;
using TcpConnectionPtr=std::shared_ptr<TcpConnection>;

void connectFunc(const TcpConnectionPtr &);
void recvmsgFunc(const TcpConnectionPtr &);
void disconnectFunc(const TcpConnectionPtr &);
void sendmsgFunc(const TcpConnectionPtr &);

Thread_Pool *p;
extern __thread LRUCache *name;
__thread vector<pair<LRUCache*,Mutex*>> *LRUManager=nullptr;

int main()
{
    Thread_Pool thread_pool(10,4);
    p=&thread_pool;
    LRUManager=new vector<pair<LRUCache*,Mutex*>>;
    p->start();
    sleep(1);
    for(auto &e:*LRUManager)
    {
        cout<<"LRUCache:"<<e.first<<"-----"<<"Mutex:"<<e.second<<endl;
    }
    cout<<"LRUCache:"<<name<<"-----"<<"Mutex:"<<_mutex<<endl;
    TcpServer server("192.168.19.128",8888,
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
    p->addTask(bind(&zwp::MyTask::process,task));
};

void disconnectFunc(const TcpConnectionPtr &tcp)
{
    cout<<*tcp<<"----disconnect"<<endl;
};

void sendmsgFunc(const TcpConnectionPtr &tcp)
{
    tcp->send("hello");
};

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

