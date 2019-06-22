#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
using namespace std;

class LRUCache {
public:
    LRUCache(int capacity)
    :capacity(capacity)
    {
        
    }
    
    string get(const string &key) {
        auto it=hash_map.find(key);
        if(it==hash_map.end())
            return nullptr;
        else
        {
            que.splice(que.begin(),que,it->second);
            it->second=que.begin();
            return que.begin()->value;
        }
    }

    struct Point
    {
        Point(string key,string value)
            :key(key),value(value){}
        string key;
        string value;
    };

    void put(string key, string value) {
        auto it=hash_map.find(key);
        if(it==hash_map.end())
        {//不在
            if(capacity==(int)hash_map.size())
            {//满了，换出操作
                auto out_it=que.back();
                hash_map.erase(out_it.key);
                que.pop_back();
            }
            //插入
            que.push_front(Point(key,value));
            hash_map.insert(make_pair(key,que.begin()));
        }
        else
        {//在，则更新value,以及位置
            que.splice(que.begin(),que,it->second);
            it->second=que.begin();
            que.begin()->value=value;
        }
    }

    list<Point> *getUpdateList(){   return &que;    }
private:
    unordered_map<string,list<Point>::iterator> hash_map;
    list<Point> que;
    int capacity;
};

