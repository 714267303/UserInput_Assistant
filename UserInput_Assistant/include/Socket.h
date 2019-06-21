#ifndef  __ZWP_SOCKET_H__
#define  __ZWP_SOCKET_H__

namespace  zwp
{

class Socket
{
public:
    Socket();

    ~Socket();
    explicit
    Socket(int fd);

    void shutdownWrite();

    int fd();

private:

    int _fd;

};

}
#endif
