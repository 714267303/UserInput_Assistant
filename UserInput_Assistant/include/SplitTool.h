#pragma once
#include <vector>
#include <iostream>
namespace zwp
{

class SplitTool
{
public:
    
    SplitTool() {};
    
    virtual
    ~SplitTool() {};
    
    virtual
    std::vector<std::string> cut(const std::string &)=0;
private:

};

}
