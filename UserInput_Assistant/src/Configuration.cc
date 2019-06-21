#include "../include/Configuration.h"
#include <iostream>

namespace zwp
{

Configuration::Auto Configuration::auto_release;
Configuration* Configuration::config=nullptr;

Configuration* Configuration::getInstance(const std::string&filepath=nullptr)
{
     if(!config)
         config=new Configuration(filepath);
     return config;
}

Configuration::Configuration(const std::string &filepath)
    :_filepath(filepath)
{
    std::ifstream ifs(filepath,std::ifstream::in|std::ifstream::binary);
    std::string key,value;
    while(ifs.good())
    {
        ifs>>key>>value;
        if(ifs)
            _configMap.insert(make_pair(key,value));
    }
}

Configuration::~Configuration(){}

void Configuration::show()
{
    for(auto &e:_configMap)
    {
        std::cout<<e.first<<":"<<e.second<<std::endl;
    }
}

}

#if 0
int main()
{
    zwp::Configuration *con=zwp::Configuration::getInstance("../conf/config");
    con->show();
    return 0;
}
#endif

