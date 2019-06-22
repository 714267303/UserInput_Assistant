#include "../include/LRUCache.h"
#include <iostream>

namespace zwp
{

string LRUCache::get(const string &key) 
{
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

void LRUCache::put(string key, string value) 
{
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

list<LRUCache::Point> *LRUCache::getUpdateList()
{ 
    return &que; 
}

}
