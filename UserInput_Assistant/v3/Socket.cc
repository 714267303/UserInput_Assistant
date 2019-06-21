#include <iostream>
#include <Myhead.h>
#include "Socket.h"
namespace zwp
{

int Socket::fd()
{
    return _fd;
}

void Socket::shutdownWrite()
{
    shutdown(_fd,SHUT_WR);
}

Socket::Socket()
{
    _fd=socket(AF_INET,SOCK_STREAM,0);
    if(-1==_fd)
    {
        perror("socket");
    }

}

Socket::~Socket()
{
    close(_fd);
}

Socket::Socket(int fd)
    :_fd(fd)
{}

}

