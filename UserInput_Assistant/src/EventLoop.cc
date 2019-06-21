#include "../include/EventLoop.h"
#include <iostream>
#include <assert.h>
#include <sys/eventfd.h>
namespace zwp
{

    using Functor=std::function<void()>;
void EventLoop::loop()
{
    isRunning=true;
    while(isRunning)
    {
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    if(isRunning)
        isRunning=false;
}

void EventLoop::addEpollFd(int fd)
{
    struct epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN;

    events.push_back(event);
    int ret=epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&event);
    if(-1==ret)
    {
        perror("epoll_ctl");
    }
}

void EventLoop::delEpollFd(int fd)
{
    int ret=epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,NULL);
    if(-1==ret)
    {
        perror("epoll_ctl");
    }
}



void EventLoop::waitEpollFd()
{
    int nready=-1;
    do
    {
        nready=epoll_wait(epoll_fd,&*events.begin(),
                          events.size(),5000);
    }while(nready==-1&&errno==EINTR);

    if(nready==-1)
    {
        perror("epoll_wait");
        return ;
    }
    else if(nready==0)
    {
        std::cout<<"time out"<<std::endl;
    }
    else
    {
        if(nready==(int)events.size())
        {
            events.resize(2*nready);
        }
        for(int i=0;i<nready;i++)
        {
            if(events[i].events==EPOLLIN)
            {
                if(events[i].data.fd==acceptor.fd())
                {
                    handleNewConnection(); 
                }
                else if(events[i].data.fd==event_fd)
                {
                    handleRead();
                    dopendingFunctor();
                }
                else
                {
                    handleMessage(events[i].data.fd);
                }
            }
        }
    }
}

int EventLoop::createEpollFd()
{
    int ret=epoll_create1(0);
    if(-1==ret)
    {
        perror("epoll_create1");
    }
    return ret;
}

void EventLoop::handleMessage(int fd)
{
    auto connect=_map.find(fd);           
    assert(connect!=_map.end());
    bool isClosed=isConnectionClosed(fd);
    if(!isClosed)
    {
        connect->second->doMsgCb();
    }
    else
    {
        connect->second->doDisConCb();
        delEpollFd(fd);
        events.pop_back();
        _map.erase(connect);
    }
}

void EventLoop::handleNewConnection()
{
    int fd=acceptor.accept();
    struct epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN;
    int ret=epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&event);

    if(-1==ret)
    {
        perror("epoll_ctl");
    }

    std::shared_ptr<TcpConnection> ptr(new TcpConnection(fd,this));
    ptr->setConCb(_connecting_cb);
    ptr->setMsgCb(_recvmsg_cb);
    ptr->setDisConCb(_disconnect_cb);

    _map.insert(std::make_pair(fd,ptr));

    ptr->doConCb();
}

bool EventLoop::isConnectionClosed(int fd)
{
    int ret=-1;
    do
    {
        char buf[1024]={0};
        ret=recv(fd,buf,sizeof(buf),MSG_PEEK);
    }while(ret==-1&&errno==EINTR);

    return ret==0;
}
void EventLoop::handleRead()
{
    int64_t ret;
    ret=::read(event_fd,&ret,sizeof(ret));
    if(ret!=sizeof(ret))
    {
        perror("handleRead");
    }
};
void EventLoop::wakeup()
{
    int64_t one=1;
    int ret=::write(event_fd,&one,sizeof(one));
    if(ret!=sizeof(one))
    {
        perror("wakeup");
    }       

};
void EventLoop::runInLoop(Functor &&f)
{
    {
        MutexLockGuard autolock(_mutex);
        funcVec.push_back(f);
    }
    wakeup();
};
void EventLoop::dopendingFunctor()
{
    std::vector<Functor> doing;
    doing.clear();
    {
        MutexLockGuard autolock(_mutex);
        doing.swap(funcVec);
    }
    for(auto &e:doing)
    {
        std::cout<<"doing"<<std::endl;
        e();
    }
};

int EventLoop::creatEventfd()
{
    int ret=::eventfd(0,0);
    if(-1==ret)
    {
        perror("eventfd");
    }
    return ret;
}

}
