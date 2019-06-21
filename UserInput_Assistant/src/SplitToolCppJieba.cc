#include "../include/SplitToolCppJieba.h"
#include <iostream>
using namespace  std;
namespace zwp
{

vector<string> SplitToolCppJieba::cut( const std::string &path )
{
    vector <string> words;   
    ifstream ifs(path);
    assert(ifs);
    
    //你好 
    stringstream oss(ios_base::app|ios_base::out);
    while(ifs.good())
    {
        char c=(char)ifs.get();
        if(ifs.good())
        {
            if(c=='\n')
                c=' ';
            oss<<c;
        }
    };

    //cout<<oss.str()<<endl;

    jieba.Cut(oss.str(),words,true);

    
    return words;
}

}
