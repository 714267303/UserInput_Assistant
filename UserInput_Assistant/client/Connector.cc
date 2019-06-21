#include <iostream>
#include "Connector.h"

namespace zwp
{

Connector::Connector(const string &ip,short port)
    :_socket()
     ,_inetaddr(ip,port)
{

}

void Connector::ready()
{
    bind();
}

void Connector::bind()
{
    socklen_t size=sizeof(sockaddr);
    int ret=::bind(_socket.fd(),(struct sockaddr*)_inetaddr.getAddrptr(),size);
    if(ret<0)
    {
        perror("bind");
    }
}

int Connector::connect()
{
    ::connect(_socket.fd(),(struct sockaddr*)_inetaddr.getAddrptr(),sizeof(struct sockaddr));
    return _socket.fd();
}

}
