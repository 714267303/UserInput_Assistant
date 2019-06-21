#include "../include/IndexProducer.h"
#include "../include/Configuration.h"
#include <fstream>
#include <iostream>
#include <assert.h>
#include <sstream>
#include <unordered_set>
using namespace std;
namespace zwp
{

IndexProducer * IndexProducer::indexProducerPtr=nullptr;
IndexProducer::Auto IndexProducer::autoRelease;

IndexProducer::IndexProducer(const std::string &dir,const std::string &outdir)
    :_dir(dir)
    ,_outdir(outdir)
{
    std::ifstream ifs(_dir);
    assert(ifs);

    std::stringstream ss;
    while(ifs.good())
    {
        char c=(char)ifs.get();
        if(ifs.good())
            ss<<c;
    }

    string key;
    int value;
    ss>>value;
    while(ss.good())
    {
        ss>>key;
        ss>>value;
        if(ss.good())
        {
            words.push_back(make_pair(key,value)); 
        }
    }
    //pair<string,int> p;
}

void IndexProducer::build_index()
{
    //对于单词，
    int china_Num=0;
    for(int i=0;i!=26;i++)
    {
        set<int> line;
        stringstream letter;
        char ch='a'+i;

        for(int i=0;i<(int)words.size();i++)
        {
            if(words[i].first.find(ch)!=std::string::npos)
            {
                line.insert(i);
                continue;
            };
        }

        letter<<ch;
        index.push_back(make_pair(letter.str(),line));
        str2index.insert(make_pair(letter.str(),china_Num++));
    }
    //对于汉字，使用一个集合进行存储已经出现的汉字
    int flag1=1<<5;
    int flag2=1<<6;
    int flag3=1<<7;

    unordered_set<string> cheakSet;
    for(int i=0;i<(int)words.size();i++)
    {
        string tobe_cheak=words[i].first;
        const char* pos=tobe_cheak.c_str();
        while(*pos)
        {

            stringstream ss;
            if((*pos&flag1)&&(*pos&flag2)&&(*pos&flag3))
            {//中文字符
                for(int i=0;i<Configuration::getInstance("")->getCodeLenth();i++)
                {
                    ss<<*pos;      
                    pos++;
                }
            }
            else
            {
                pos++;
            }
            string s=ss.str();
            if(s.size())
            {
                set<int> line;
                auto result=cheakSet.insert(s);
                if(result.second)//插入成功，说明不在查重集合中
                {
                    for(int i=0;i<(int)words.size();i++)
                    {
                        if(words[i].first.find(s)!=std::string::npos)
                        {
                            line.insert(i);
                        };
                    }
                    index.push_back(make_pair(s,line));
                    str2index.insert(make_pair(s,china_Num++));
                }
            }
        }
    }

//不能清空！后面还有用

    /* { */
    /*     //建好索引后清空words */
    /*     vector<pair<string ,int>> temp; */
    /*     temp.swap(words); */
    /* } */

}

void IndexProducer::store_index()
{
    ofstream ofs(_outdir);
    for(auto &e:index)
    {
        ofs<<e.first<<" ";
        for(auto &v:e.second)
        {
            ofs<<v<<" ";
        }
        ofs<<endl;
    }
#if 0
    for(auto &e:str2index)
    {
        cout<<e.first<<":"<<e.second<<endl;
    }
#endif
}

IndexProducer * IndexProducer::getInstance(const std::string &dir,const std::string &outdir)
{
   if(!indexProducerPtr)
   {
        indexProducerPtr=new IndexProducer(dir,outdir);
        indexProducerPtr->build_index();
        indexProducerPtr->store_index();
   }
   return indexProducerPtr;
}

}



