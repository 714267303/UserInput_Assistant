#ifndef  __ZWP_TASK_H__
#define  __ZWP_TASK_H__
#include "TcpConnection.h"
#include "IndexProducer.h"
#include "Configuration.h"
#include "MyResult.h"
#include "Tool.h"
#include "LRUCache.h"
#include "Thread.h"

#include <unistd.h>
#include <iostream>
#include <sstream>
#include <queue>
#include <json/json.h>

namespace  zwp
{
extern __thread zwp::LRUCache *name;
extern __thread zwp::Mutex *_mutex;
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

        int characterNum;
        set<int> cheak_line;
        vector<string> wordset=Tool::getNumOfchars(_msg,characterNum);

        Json::Value value;

        cout<<zwp::name<<endl;
#if 1
        zwp::_mutex->lock();
        string findCache=zwp::name->get(_msg);
        zwp::_mutex->unlock();
        cout<<"findCache:"<<findCache<<" size:"<<findCache.size()<<endl;

        if(!findCache.size())
        {
            for(int i=0;i<characterNum;i++)
            {
                auto it=str2index.find(wordset[i]);
                if(it==str2index.end())
                {

                }
                else
                {
                    set<int> line=index[it->second].second;
                    for(auto &i:line)
                    {
                        auto ret=cheak_line.insert(i);
                        if(ret.second)
                        {
                            std::stringstream s;
                            s<<words[i].first;
                            int tempNum;
                            Tool::getNumOfchars(s.str(),tempNum);
                            if(tempNum<characterNum)
                            {
                                continue;
                            }
                            s<<" ";
                            //compute iDist
                            string temp=s.str(); 
                            //------
                            cout<<temp<<":"<<Tool::cac_Dist(_msg,temp)<<std::endl;
                            results.push(MyResult(temp,words[i].second,Tool::cac_Dist(_msg,temp)));
                        }
                    }
                }
            }
            //encode
            auto k=zwp::Configuration::getInstance("")->getConfigMap().find("K")->second;  
            int k2i=atoi(k.c_str());

            for(int i=0;i<k2i&&results.size();i++)
            {
                string pos(std::to_string(i));
                value[pos]=results.top()._word;
                results.pop();
                //ss<<results.top()._word<<" ";
            }
        }
        else
        {
            _conn->sendInLoop(findCache); 
            return;
        }
        string styleStr(value.toStyledString());
        _conn->sendInLoop(styleStr);
        zwp::_mutex->lock();
        zwp::name->put(_msg,styleStr);
        zwp::_mutex->unlock();
#endif
    }

private:
    string _msg;
    TcpConnection::TcpConnectionPtr _conn;
};

}
#endif
