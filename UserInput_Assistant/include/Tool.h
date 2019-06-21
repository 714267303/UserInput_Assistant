#pragma once
#include <string>
#include <iostream>
#include <vector>
using namespace std;
namespace zwp
{

class Tool
{
public:
    static std::vector<std::string> getNumOfchars(const std::string &s,int &ret_num);
    static int cac_Dist(std::string& lhs,std::string& rhs);
private:
    Tool() {}
    ~Tool() {}
};

}
