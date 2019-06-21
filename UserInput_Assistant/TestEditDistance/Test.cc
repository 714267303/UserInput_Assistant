#include "include/Configuration.h"
#include <iostream>
#include <vector>

std::vector<std::string> getNumOfchars(const std::string &s,int &ret_num);
int cac_Dist(std::string lhs,std::string rhs)
{
    //const int llen=lhs.size()+1;
    //const int rlen=rhs.size()+1;
    
    int llen,rlen;
    std::vector<std::string> lhs_vec=getNumOfchars(lhs,llen);
    std::vector<std::string> rhs_vec=getNumOfchars(rhs,rlen);

    ++llen;++rlen;
    std::cout<<"llen:"<<llen<<std::endl;
    std::cout<<"rlen:"<<rlen<<std::endl;

    int **p=(int **)malloc(sizeof(int*)*(llen));
    for(int i=0;i<llen;i++)
    {
        p[i]=(int *)malloc(sizeof(int)*(rlen));
    }

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

    for(int i=0;i<llen;i++)
    {
        for(int j=0;j<rlen;j++)
        {
            std::cout<<p[i][j]<< " ";
        }
        std::cout<<std::endl;
    }
    int result=p[llen-1][rlen-1];

    for(int i=0;i<llen;i++)
    {
        free(p[i]);
    }
    free(p);
    return result;
}

std::vector<std::string> getNumOfchars(const std::string &s,int &ret_num)
{
    int flag1=1<<7;
    int flag2=1<<6;
    int flag3=1<<5;

    const char *p=s.c_str();
    int num=0;
    std::vector<std::string> vec;
    
    auto configmap=zwp::Configuration::getInstance("conf/config")->getConfigMap();
    int code=atoi(configmap.find("ENCODE")->second.c_str());
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

int main()
{
    int ret;

    std::vector<std::string> vec=getNumOfchars("我好厉害233333",ret);
    for(auto&e:vec)
    {
        std::cout<<e<<std::endl;
    }
    std::cout<<"ret:"<<ret<<std::endl;

    while(1)
    {
        std::string a,b;
        std::cin>>a>>b;
        std::cout<<cac_Dist(a,b)<<std::endl;
    }
    return 0;
}

