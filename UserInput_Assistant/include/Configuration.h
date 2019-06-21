#pragma once

#include <map>
#include <fstream>
#include <iostream>
namespace zwp
{

class Configuration
{
public:
    class Auto
    {
    public:
        Auto(){}
        ~Auto()
        {
            if(config)
                delete config;
        }
    };
    void show();
    int getCodeLenth(){ return codeLength; }

    std::multimap<std::string,std::string> & getConfigMap()
    {
        return _configMap;
    };
    
    static Configuration *getInstance(const std::string &filepath);

private:

    int codeLength=3;
    static Configuration * config;
    static Auto auto_release;
    std::string _filepath;
    std::multimap<std::string,std::string> _configMap;

private:

    Configuration(const std::string &filepath);
    ~Configuration() ;

};


}
