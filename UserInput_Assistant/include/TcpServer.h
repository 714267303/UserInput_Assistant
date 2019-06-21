#ifndef __ZWP_TCPSERVER_H__
#define __ZWP_TCPSERVER_H__
#include "EventLoop.h"
namespace zwp
{

class TcpServer
{
    using TcpConnectionPtr=std::shared_ptr<TcpConnection>;
    using callback=std::function<void(const TcpConnectionPtr &)>;
public:
    TcpServer(string ip,short port,
              callback connecting_cb,
              callback recvmsg_cb,
              callback disconnect_cb,
              callback sendmsg_cb)
    :_acceptor(ip,port)
     ,_eventloop(_acceptor,connecting_cb,recvmsg_cb,disconnect_cb
                 ,sendmsg_cb)
    {
        _acceptor.ready();
    }
    ~TcpServer() {}
    void start();
    void stop();
private:
    Accepter  _acceptor;        
    EventLoop _eventloop;
};

}
#endif
