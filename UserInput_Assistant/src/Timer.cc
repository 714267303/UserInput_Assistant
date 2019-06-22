#include "../include/Timer.h"
#include <sys/timerfd.h>
#include <poll.h>
#include <iostream>
#include <Myhead.h>
namespace  zwp
{

int Timer::createTimerfd()
{
    int fd=timerfd_create(CLOCK_REALTIME,0);
    if(-1==fd)
    {
        perror("createTimerfd");
    }
    return fd;
}

void Timer::settime(int _initTime,int _intervalTime)
{
    struct itimerspec time_val;
    time_val.it_interval.tv_sec=_intervalTime;
    time_val.it_interval.tv_nsec=0;
    time_val.it_value.tv_sec=_initTime;
    time_val.it_value.tv_nsec=0;

    int ret=timerfd_settime(_fd,0,&time_val,nullptr);
    if(-1==ret)
    {
        perror("settime");
    }
}

void Timer::start()
{
    isStarted=true;
    settime(_initTime,_intervalTime);

    struct pollfd pfd; 
    pfd.fd=_fd;
    pfd.events=POLLIN;

    while(isStarted)
    {
        int nready=::poll(&pfd,1,5000);
        if(nready==-1&&EINTR)
        {
            continue;
        }
        else if(nready ==-1)
        {
            perror("poll");
            return ;
        }
        else if(nready==0)
        {
            std::cout<<"poll time out"<<std::endl;
        }
        else
        {
            handleRead();
            if(_cb)
                _cb();
        }
    }
}

void Timer::stop()
{
    if(isStarted)
    {
        settime(0,0);
        isStarted=false;
    }
}

void Timer::handleRead()
{
    int64_t ret;
    read(_fd,&ret,sizeof(ret));
}

}
