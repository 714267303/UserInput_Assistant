#include <iostream>
#include <json/json.h>
int main()
{
    std::string test="{\"name\":\"小明\",\"age\":18}";
    Json::Reader reader;
    Json::Value  value;

#if 0
    std::cout<<"case 1"<<std::endl;
    if(reader.parse(test,value))
    {
        auto vec=value.getMemberNames();
        for(auto &e:vec)
        {
            std::cout<<e<<":"<<value[e];
        }
    }
#endif 
    std::cout<<"case 2"<<std::endl;

    Json::Value json;

   /* json["name"]=Json::Value("老王"); */
   /*  json["age"]=Json::Value(18); */

    std::string s=json.toStyledString();
    std::cout<<s<<std::endl;

    std::cout<<"case 3"<<std::endl;
    if(reader.parse(s,value))
    {
        auto vec=value.getMemberNames();
        for(auto &e:vec)
        {
            std::cout<<e<<":"<<value[e];

        }
    }
    return 0;
}

