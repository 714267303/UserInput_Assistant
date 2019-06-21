#include "Connector.h"
#include "SocketIO.h"
#include <iostream>
#include <json/json.h>
using namespace  std;
int main()
{
    zwp::Connector conn("192.168.19.128",8888); 
    int fd=conn.connect();
    char buf[1024]={0};
    int ret;//= read(fd,buf,sizeof(buf));
    string word;
    zwp::SocketIO sio(fd);
    Json::Reader reader;
    Json::Value value;
    while(cin>>word)
    {
        memset(buf,0,sizeof(buf));
        ret= sio.writen(word.c_str(),strlen(word.c_str()));
        int len;
        sio.read32int(len);
        cout<<"len:"<<len<<endl; 
        sio.readn(buf,len);
        string s(buf);
        reader.parse(s,value);
        auto vec=value.getMemberNames();
        for(auto &i:vec)
        {
            cout<<value[i].asString();
        }
    }
    return 0;
}

