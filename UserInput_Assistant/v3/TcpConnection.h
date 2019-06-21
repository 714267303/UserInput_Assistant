#ifndef  __ZWP_TCPCONNECTION_H__
#define  __ZWP_TCPCONNECTION_H__
#include "InetAddr.h"
#include "Socket.h"
#include "SocketIO.h"
#include <functional>
#include <memory>
namespace zwp
{
class EventLoop;
class TcpConnection
    :public std::enable_shared_from_this<TcpConnection>
{
public:
    using Functor=std::function<void()>;
    using TcpConnectionPtr=std::shared_ptr<TcpConnection>;
    using callback=std::function<void(const TcpConnectionPtr&)>;
    TcpConnection(int fd,EventLoop*) ;
    ~TcpConnection(){} ;
    string recv();
    void send(const string &);
    void disConnect();
    InetAddr getLocalAddr();
    InetAddr getPeerAddr();
    friend std::ostream& operator <<(std::ostream &,TcpConnection &);
    void setMsgCb(callback );
    void setConCb(callback );
    void setDisConCb(callback );
    
    void doMsgCb();
    void doConCb();
    void doDisConCb();
    void sendInLoop(const string& msg);
private:
    InetAddr _localAddr;
    InetAddr _peerAddr;
    Socket _socket;
    SocketIO _socketIO;
    bool isRunning;
    int _fd;
    int eventfd; 
    callback msg_cb;
    callback con_cb;
    callback discon_cb;

    EventLoop *_loop;
};

}
#endif
