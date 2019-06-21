#include "../include/SocketIO.h"
#include <iostream>
#include <Myhead.h>
#include <string>

namespace zwp
{

SocketIO::SocketIO(int fd)
    :_fd(fd){}
int SocketIO::writen(const char *buf,int len)
{
    int left=len;
    const char *p=buf;
    while(left>0)
    {
        int ret=send(_fd,p,left,0);
        if(ret==-1&&errno==EINTR) 
            continue;
        else if(ret<0)
            perror("send");
        else if(ret==0)
        {
            return len-left;
        }
        else
        {
            p+=ret;
            left-=ret;
        }
    }
    return len - left;
}
int SocketIO::readn(char *buf,int len)
{
    int left=len;
    char *p=buf;
    while(left>0)
    {
        int ret=read(_fd,p,left);
        if(ret==-1&&errno==EINTR) 
            continue;
        else if(ret==-1)
        {
            perror("recv");
            return -1;
        }
        else if(ret==0)
        {
            return len-left;
        }
        else
        {
            p+=ret;
            left-=ret;
        }
    }
    return len - left;
}
void SocketIO::write32int(int len)
{
    std::string s=std::to_string(len);
    writen(s.c_str(),sizeof(int));
}
void SocketIO::read32int(int &len)
{
    char buf[20]={0};
    readn(buf,sizeof(int));
    std::string s(buf);
    len=std::stoi(s);
}//测试

int SocketIO::readline(char *buf,int maxlen)
{
    auto recvPeek=[](int fd,char *buf,int maxlen)
    {
        int ret=-1;
        do
        {
            ret=recv(fd,buf,maxlen,MSG_PEEK);

        }while(ret==-1&&errno==EINTR);
        return ret;
    };

    int pos=recvPeek(_fd,buf,maxlen);
    if(pos==-1)
    {
        std::cout<<"recvPeek erro"<<std::endl;
        return -1;
    }

    for(int i=0;i<maxlen;i++)
    {
        if(buf[i]=='\n')
        {
            pos=i+1;
            break;
        }
    }
    readn(buf,pos);
    return pos;
}

}
