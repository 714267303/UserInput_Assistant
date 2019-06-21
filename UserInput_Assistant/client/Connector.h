#ifndef __ZWP_CONNECTOR_H__
#define __ZWP_CONNECTOR_H__

#include "InetAddr.h"
#include "Socket.h"

namespace zwp
{

class Connector
{
public:
    Connector(const string& ip,short port);
    ~Connector(){};
    void ready();
    int connect();
private:
    void bind();
private:
    Socket _socket;
    InetAddr _inetaddr;
};

}

#endif
