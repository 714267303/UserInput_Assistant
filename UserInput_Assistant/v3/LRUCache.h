#pragma once
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
private:
    unordered_map<string,list<Point>::iterator> hash_map;
    list<Point> que;
    int capacity;
};

}
