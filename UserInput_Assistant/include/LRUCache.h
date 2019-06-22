#pragma once
#include "../include/Configuration.h"
#include "../include/Mutex.h"
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
using namespace std;

namespace  zwp
{

class LRUCache {
public:
    LRUCache(int capacity)
    :capacity(capacity)
    {
        threadNum=atoi(Configuration::getInstance("")->getConfigMap().find("THREADNUM")->second.c_str());
        UpDateNum=capacity/threadNum;
    }
    
    string get(const string &key) ;

    struct Point
    {
        Point(string key,string value)
            :key(key),value(value){}
        string key;
        string value;
    };

    void put(string key, string value);

    list<Point> *getUpdateList();
    int getUpdateNum(){ return UpDateNum;   }
private:
    unordered_map<string,list<Point>::iterator> hash_map;
    list<Point> que;
    static int threadNum;
    static int UpDateNum;
    int capacity;
};



}
