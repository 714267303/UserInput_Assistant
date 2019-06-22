#ifndef __ZWP_TIMER_H__
#define __ZWP_TIMER_H__
#include <functional>
namespace  zwp
{

class Timer
{
public:
    using callback=std::function<void()>;
    Timer(int init,int interval,callback&& cb)
    :isStarted(false)
     ,_fd(createTimerfd())
     ,_initTime(init)
     ,_intervalTime(interval)
     ,_cb(move(cb))
    {}
    ~Timer() {}
    void start();
    void stop();
    void settime(int,int);
private:
    int createTimerfd();
    void handleRead();
private:
    bool isStarted;
    int _fd;
    int _initTime;
    int _intervalTime;
    callback _cb;
};

}

#endif
