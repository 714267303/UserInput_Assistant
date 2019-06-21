#include "../include/DictProducer.h"
#include "../include/SplitTool.h"
#include "../include/Configuration.h"
#include <iostream>
#include <assert.h>

namespace zwp
{

void DictProducer::build_dict()
{
    if(_isCn)
        build_Corresponding_dict("CN");
    else
        build_Corresponding_dict("EN");
}

void DictProducer::build_Corresponding_dict(const std::string &prefix)
{
    auto range=Configuration::getInstance("../conf/config")->getConfigMap().equal_range(prefix);
    auto it=range.first;
    while(it!=range.second)
    {
        //std::cout<<"file:"<<it->second<<std::endl;
        deal_file(_dic,it->second);
        it++;
    }
    //存入文件
    store();
}

void DictProducer::deal_file(std::map<std::string,int> &dic,const std::string &path)
{
    std::vector<std::string> dict=_splitTool->cut(path);

    //统计词频
    for(auto &e:dict)
    {
        auto it=dic.insert(make_pair(e,1));
        if(it.second)
        {
            //插入成功
        }
        else
        {
            //std::cout<<it.first->first<<" is exits "<<it.first->second++;
            it.first->second++;
        }
    }
};


void DictProducer::store()
{
    std::ofstream ofs(_dir,std::ios::trunc);
    assert(ofs);

    for(auto it=_dic.begin();it!=_dic.end();it++)
    {
       ofs<<it->first<<" "<<it->second<<std::endl; 
    }
}

}

