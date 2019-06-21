#pragma once
#include <vector>
#include <set>
#include <string>
#include <unordered_map>
namespace zwp
{

class IndexProducer
{
public:

    class Auto
    {
    public :
        Auto()
        {}
        ~Auto()
        {
            if(indexProducerPtr)
                delete indexProducerPtr;
        }
    };

    std::vector<std::pair<std::string,int>> getWords(){ return words; }
    std::vector<std::pair<std::string,std::set<int>>> getIndex(){ return index;  }
    std::unordered_map<std::string,int> getStr2index(){ return str2index; }
    static IndexProducer *getInstance(const std::string &dir=nullptr,const std::string &outdir=nullptr);
private:
    void build_index();
    void store_index();
    ~IndexProducer() {}
    IndexProducer(const std::string &dir,const std::string &outdir);
private:
    static IndexProducer * indexProducerPtr;
    static Auto autoRelease;
private:
    std::string _dir;
    std::string _outdir;
private:
    std::vector<std::pair<std::string,int>> words;
    std::vector<std::pair<std::string,std::set<int>>> index;
    std::unordered_map<std::string,int> str2index;
};

}
