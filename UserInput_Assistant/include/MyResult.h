#pragma once
#include <string>
class MyResult
{
public:
    MyResult(std::string word,
             int iFreq,
             int iDist=0)
    :_word(word)
    ,_iFreq(iFreq)
    ,_iDist(iDist)
    {}
    ~MyResult() {}
    
    std::string _word;
    int _iFreq;
    int _iDist;
};

