#pragma once

#include <unordered_map>
#include "basic/BasicMacro.h"
#include <list>
#include <utility>

using std::unordered_map;
using std::list;
using std::pair;

USI_NS_BEGIN

struct LruCache
{
    LruCache(U32 cap) : capacity(cap) {}

    S32 get(S32 key)
    {
        auto iter = cacheMap.find(key);
        if(iter == cacheMap.end())
            return INVALID_U32;
        
        auto item = iter->second->second;
        cacheList.push_front(*(iter->second));
        cacheList.erase(iter->second);
        cacheMap[key] = cacheList.begin();

        return item;
    }

    void put(S32 key, S32 val)
    {
        auto iter = cacheMap.find(key);
        if(iter != cacheMap.end())
        {
            cacheList.push_front({key, val});
            cacheList.erase(iter->second);
            cacheMap[key] = cacheList.begin();

            return ;
        }

        if(cacheList.size() == capacity)
        {
            auto last = cacheList.back();
            cacheMap.erase(last.first);
            cacheList.pop_back();
        }
        cacheList.push_front({key, val});
        cacheMap[key] = cacheList.begin();
    }

private:
    U32 capacity;
    unordered_map<S32, list<pair<S32, S32>>::iterator> cacheMap;
    list<pair<S32, S32>> cacheList;
};

USI_NS_END


