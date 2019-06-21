#ifndef __ZWP_EVENTLOOP_H__
#define __ZWP_EVENTLOOP_H__ 
#include "Accepter.h"
#include "TcpConnection.h"
#include "Mutex.h"
#include <map>
#include <functional>
#include <vector>
#include <event.h>
namespace zwp
{

class EventLoop
{
    using Functor=std::function<void()>;
    using TcpConnectionPtr=std::shared_ptr<TcpConnection>;
    using callback=std::function<void(const TcpConnectionPtr &)>;
public:
    EventLoop(Accepter &ac,
              callback connecting_cb,
              callback recvmsg_cb,
              callback disconnect_cb,
              callback sendmsg_cb)
    :acceptor(ac)
    ,isRunning(false)
    ,epoll_fd(createEpollFd())
    ,_connecting_cb(connecting_cb)
    ,_recvmsg_cb(recvmsg_cb)
    ,_disconnect_cb(disconnect_cb)
    ,_sendmsg_cb(sendmsg_cb)
    ,event_fd(creatEventfd())
    {
        events.clear();
        _map.clear();
        funcVec.clear();
        events.reserve(10);
        addEpollFd(acceptor.fd());
        addEpollFd(event_fd);
    }
    ~EventLoop() {}
    void loop();
    void unloop();
    void addEpollFd(int);
    void delEpollFd(int);
    void waitEpollFd();
    int  createEpollFd();   
    void handleMessage(int fd);
    void handleNewConnection();
    bool isConnectionClosed(int fd);

    void handleRead();
    void wakeup();
    void runInLoop(Functor &&);
    void dopendingFunctor();
    int creatEventfd();
private:
    Accepter &acceptor;    
    bool isRunning;
    int epoll_fd;
    std::map<int,std::shared_ptr<TcpConnection>> _map;
    std::vector<struct epoll_event> events;
    callback  _connecting_cb;
    callback  _recvmsg_cb;
    callback  _disconnect_cb;
    callback  _sendmsg_cb;

    Mutex _mutex;
    int event_fd;
    std::vector<Functor> funcVec;
};
}
#endif
