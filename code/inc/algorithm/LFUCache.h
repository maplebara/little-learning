#pragma once

#include <unordered_map>
#include <set>
#include "basic/BasicMacro.h"

using std::unordered_map;
using std::set;

USI_NS_BEGIN

class LFUCache {
    struct Elem
    {
        int val;
        int key;
        int count;
        int time;

        bool operator<(const Elem& rhs) const
        {
            if(count == rhs.count)
                return this->time < rhs.time;
            
            return this->count < rhs.count;
        }
    };

    using setItor = std::set<Elem>::iterator;
    unordered_map<int, setItor> datas;
    set<Elem> rb;
    int cap;
    int g_time;

public:
    LFUCache(int capacity) {
        cap = capacity;
        g_time = 0;
    }
    
    int get(int key) {
        auto itor = datas.find(key);
        if(itor == datas.end()) return -1;

        auto it = itor->second;
        auto temp = rb.insert({it->val, it->key, it->count + 1, ++g_time});
        rb.erase(it);
        datas[key] = temp.first;

        return temp.first->val;
    }
    
    void put(int key, int value) {
        if(cap == 0) return ;

        auto itor = datas.find(key);
        if(itor != datas.end())
        {
            auto it = itor->second;
            auto temp = rb.insert({value, it->key, it->count + 1, ++g_time});
            rb.erase(it);
            datas[key] = temp.first;
            return ;
        }

        if(datas.size() == cap)
        {
            int key_v = rb.begin()->key;
            datas.erase(key_v);
            rb.erase(rb.begin());
        }
        auto temp = rb.insert({value, key, 1, ++g_time});
        datas[key] = temp.first;
    }
};

USI_NS_END
