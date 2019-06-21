#ifndef  __ZWP_TASK_H__
#define  __ZWP_TASK_H__
#include "TcpConnection.h"

#include <unistd.h>
#include <iostream>

namespace  zwp
{

struct data
{
    data(string msg)
    :msg(msg){}
    string msg;
};
class MyTask
{
    friend class Thread_Pool;
public:
    MyTask(string msg,TcpConnection::TcpConnectionPtr conn)
    :_msg(msg)
    ,_conn(conn){}
    ~MyTask() {}
    void process()
    {
        //decode
        //compute
        //encode
        _conn->sendInLoop(_msg);
    }
private:
    string _msg;
    TcpConnection::TcpConnectionPtr _conn;
};

}
#endif
