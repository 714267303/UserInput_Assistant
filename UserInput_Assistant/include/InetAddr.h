#ifndef  __ZWP_INETADDR_H__
#define  __ZWP_INETADDR_H__

#include <Myhead.h>
#include <string>
using std::string;

namespace zwp
{

class InetAddr
{

public:

    explicit
    InetAddr(unsigned short port);

    InetAddr(sockaddr_in addr);

    InetAddr(const string &ip,unsigned short port);

    struct sockaddr_in *getAddrptr();

    string ip();

    short port();
    ~InetAddr(){} ;
    
private:
    struct sockaddr_in _addr;
};

}
#endif
