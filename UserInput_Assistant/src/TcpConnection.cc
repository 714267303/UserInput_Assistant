#include "../include/TcpConnection.h"
#include "../include/EventLoop.h"
#include <Myhead.h>
#include <iostream>
#include <sys/socket.h>
#define MTU 65536
namespace zwp
{

TcpConnection::TcpConnection(int fd,EventLoop *p)
    : _localAddr(getLocalAddr())
     ,_peerAddr(getPeerAddr())
      ,_socket(fd)
     ,_socketIO(fd)
     , _fd(fd)
    ,_loop(p)
{  
}

InetAddr TcpConnection::getLocalAddr()
{
    struct sockaddr_in sock_in; 
    socklen_t len;
    getsockname(_fd,(struct sockaddr*)&sock_in,&len);

    return InetAddr(sock_in);
}

InetAddr TcpConnection::getPeerAddr()
{
    struct sockaddr_in sock_in; 
    socklen_t len;
    getpeername(_fd,(struct sockaddr*)&sock_in,&len);
    
    return InetAddr(sock_in);
}

void TcpConnection::disConnect()
{
    _socket.shutdownWrite();

}

string TcpConnection::recv()
{
    char buf[MTU]={0};
    _socketIO.readline(buf,MTU);
    return string(buf);
}   

void TcpConnection::send(const string &msg)
{
    int len=strlen(msg.c_str());
    _socketIO.write32int(len);
    _socketIO.writen(msg.c_str(),len);
}

std::ostream& operator <<(std::ostream &os,TcpConnection &tcp)
{
    os<<tcp._localAddr.ip()<<"------->"<<tcp._peerAddr.ip();
    return os;    
}

void TcpConnection::setMsgCb(callback cb)
{
    msg_cb=move(cb);
};
void TcpConnection::setConCb(callback cb)
{
    con_cb=move(cb);
};
void TcpConnection::setDisConCb(callback cb)
{
    discon_cb=move(cb);
};

void TcpConnection::doMsgCb()
{
    if(msg_cb)
    {
        msg_cb(shared_from_this());
    }
};
void TcpConnection::doConCb()
{
    if(con_cb)
    {
        con_cb(shared_from_this());
    }
};
void TcpConnection::doDisConCb()
{
    if(discon_cb)
    {
        discon_cb(shared_from_this());
    }
};

void TcpConnection::sendInLoop(const string& msg)
{
    _loop->runInLoop(bind(&TcpConnection::send,this,msg));
}

}
