#include "../include/Tool.h"
#include <iostream>

namespace zwp
{

std::vector<std::string> Tool::getNumOfchars(const std::string &s,int &ret_num)
{
    int flag1=1<<7;
    int flag2=1<<6;
    int flag3=1<<5;

    const char *p=s.c_str();
    int num=0;
    std::vector<std::string> vec;

    int code=3;
    while(*p)
    {
        if((*p)&flag1&&(*p)&flag2&&(*p)&flag3)
        {
            std::string s(p,code);
            p+=code;
            vec.push_back(s);
        }
        else
        {
            vec.push_back(std::string(p,1));
            ++p;
        }
        num+=1;
    }
    ret_num=num;
    return vec;
}

int Tool::cac_Dist(std::string &lhs,std::string &rhs)
{
    int llen,rlen;
    std::vector<std::string> lhs_vec=getNumOfchars(lhs,llen);
    std::vector<std::string> rhs_vec=getNumOfchars(rhs,rlen);

    ++llen;++rlen;
    int p[501][501];

    for(int i=0;i<rlen;i++)
    {
        p[0][i]=i;
    }

    for(int i=0;i<llen;i++)
    {
        p[i][0]=i;
    }


    for(int i=1;i<llen;i++)
    {
        for(int j=1;j<rlen;j++)
        {
            if(lhs_vec[i-1]==rhs_vec[j-1])
            {
                p[i][j]=p[i-1][j-1];
            }
            else
            {
                p[i][j]=std::min(p[i-1][j]+1,p[i-1][j-1]+1);
                p[i][j]=std::min(p[i][j-1]+1,p[i][j]);
            }
        }
    }
    return p[llen-1][rlen-1];
}

}
