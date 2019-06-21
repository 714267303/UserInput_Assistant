#include <iostream>
#include "Accepter.h"

namespace zwp
{

Accepter::Accepter(short port)
    :_socket()
    ,_inetaddr(port)
{
        
}

Accepter::Accepter(const string &ip,short port)
    :_socket()
     ,_inetaddr(ip,port)
{

}

void Accepter::ready()
{
    setReuseaddr(true);
    setReuseport(true);
    bind();
    listen();
}

void Accepter::setReuseaddr(bool on)
{
    int one=on;
    int ret=setsockopt(_socket.fd(),SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));
    if(ret<0)
    {
        perror("setReuseaddr");
    }
}

void Accepter::setReuseport(bool on)
{
    int one=on;
    int ret=setsockopt(_socket.fd(),SOL_SOCKET,SO_REUSEPORT,&one,sizeof(one));
    if(ret<0)
    {
        perror("setReuseport");
    }
}

void Accepter::bind()
{
    socklen_t size=sizeof(sockaddr);
    int ret=::bind(_socket.fd(),(struct sockaddr*)_inetaddr.getAddrptr(),size);
    if(ret<0)
    {
        perror("bind");
    }
}

void Accepter::listen()
{
    int ret=::listen(_socket.fd(),10);
    if(ret<0)
    {
        perror("listen");
    }
}

int Accepter::accept()
{
    int fd=::accept(_socket.fd(),NULL,NULL);
    return fd; 
}

int Accepter::fd()
{
    return _socket.fd();
}

}
