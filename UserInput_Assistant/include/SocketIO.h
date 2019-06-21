#ifndef  __ZWP_SOCKETIO_H__
#define  __ZWP_SOCKETIO_H__

namespace zwp
{

class SocketIO
{
public:
    SocketIO(int) ;
    ~SocketIO(){} ;
    int writen(const char *buf,int len);
    int readn(char *buf,int len);
    void write32int(int );
    void read32int(int &);
    int readline(char *buf,int maxlen);
private:
    int _fd;
};

}
#endif
