#include "../include/InetAddr.h"
#include <iostream>

namespace zwp
{

InetAddr::InetAddr(unsigned short port)
{
    ::memset(&_addr,0,sizeof(sockaddr));
    _addr.sin_family=AF_INET;
    _addr.sin_port=htons(port);
    _addr.sin_addr.s_addr=INADDR_ANY;
}

InetAddr::InetAddr(sockaddr_in addr)
:_addr(addr)
{
}

InetAddr::InetAddr(const string &ip,unsigned short port)
{
    _addr.sin_family=AF_INET;
    _addr.sin_port=htons(port);
    inet_aton(ip.c_str(),&_addr.sin_addr);
}

struct sockaddr_in *InetAddr::getAddrptr()
{
    return &_addr;
}

string InetAddr::ip()
{
    return inet_ntoa(_addr.sin_addr);
}

short InetAddr::port()
{
    return ntohs(_addr.sin_port);
}

}
