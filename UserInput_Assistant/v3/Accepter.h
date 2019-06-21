#ifndef __ZWP_ACCEPTER_H__
#define __ZWP_ACCEPTER_H__

#include "InetAddr.h"
#include "Socket.h"

namespace zwp
{

class Accepter
{
public:
    Accepter(short port);
    Accepter(const string& ip,short port);
    ~Accepter(){};
    int accept();
    void ready();
    int fd();
private:
    void bind();
    void listen();
    void setReuseport(bool on);
    void setReuseaddr(bool on);
private:
    Socket _socket;
    InetAddr _inetaddr;
};

}

#endif
