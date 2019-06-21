#ifndef  __ZWP_TASK_H__
#define  __ZWP_TASK_H__
#include "TcpConnection.h"
#include "IndexProducer.h"
#include "Configuration.h"
#include "MyResult.h"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <queue>
#include <json/json.h>

namespace  zwp
{

struct data
{
    data(string msg)
        :msg(msg){}
    string msg;
};

class MyCompare
{
public:
    bool operator()(MyResult &lhs,MyResult &rhs);
};

bool MyCompare::operator()(MyResult &lhs,MyResult &rhs)
{
    if(lhs._iDist==rhs._iDist)
    {
        return lhs._iFreq<rhs._iFreq;
    }
    else
        return lhs._iDist>rhs._iDist;
} 

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
        auto index=IndexProducer::getInstance("","")->getIndex();
        auto str2index=IndexProducer::getInstance("","")->getStr2index(); 
        auto words=IndexProducer::getInstance("","")->getWords();

        std::priority_queue<MyResult,std::vector<MyResult>,MyCompare> results;

        auto it=str2index.find(_msg);
        if(it==str2index.end())
        {
            _msg="???";
            _conn->sendInLoop(_msg);
            return ;
        }
        else
        {
            auto line=index[it->second].second;
            for(auto &i:line)
            {
                std::stringstream s;
                s<<words[i].first<<" ";
                //compute iDist
                //------
                //
                results.push(MyResult(s.str(),words[i].second,0));
            }
        }
        //encode
        auto k=zwp::Configuration::getInstance("")->getConfigMap().find("K")->second;  
        int k2i=atoi(k.c_str());
        std::stringstream ss;
        Json::Value value;

        for(int i=0;i<k2i&&results.size();i++)
        {
            string pos(std::to_string(i));
            value[pos]=results.top()._word;
            results.pop();
            //ss<<results.top()._word<<" ";
        }
        _conn->sendInLoop(value.toStyledString());
    }

private:
    string _msg;
    TcpConnection::TcpConnectionPtr _conn;
};

}
#endif
