#pragma once
#include <string>
#include <vector>
#include <map>
namespace zwp
{
class SplitTool;
class DictProducer
{
public:
    DictProducer(const std::string &dir)
        :_isCn(false)
        ,_dir(dir){};

    DictProducer(const std::string &dir,SplitTool *splitTool)
        :_isCn(true)
        ,_dir(dir)
        ,_splitTool(splitTool){};

    ~DictProducer(){};

    void store();
    void build_dict();
    void build_Corresponding_dict(const std::string &);
    void deal_file(std::map<std::string,int> &dic,const std::string &path);
    
    std::string getdir()
    {
        return _dir;
    }
private:
    std::vector<std::string> _files;
    std::map<std::string,int> _dic;
    
private:
    bool _isCn;
    std::string _dir;
    SplitTool *_splitTool;
};

}
