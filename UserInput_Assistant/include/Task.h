#ifndef  __ZWP_TASK_H__
#define  __ZWP_TASK_H__
#include "TcpConnection.h"
#include "IndexProducer.h"
#include "Configuration.h"
#include "MyResult.h"
#include "Tool.h"
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

        int characterNum;
        set<int> cheak_line;
        vector<string> wordset=Tool::getNumOfchars(_msg,characterNum);

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
                        s<<words[i].first<<" ";
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
