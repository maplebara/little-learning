#pragma once

#include "basic/BasicMacro.h"
#include <mutex>
#include <condition_variable>
#include <queue>

USI_NS_BEGIN

struct BlockingQueue
{
    BlockingQueue(int capacity) : cap(capacity) {}

    void put(int val)
    {
        std::unique_lock<std::mutex> lk(m_lock);
        not_full.wait(lk, [this]{return data.size() < cap;});
        data.push(val);
        not_empty.notify_all();
    }

    int pop()
    {
        std::unique_lock<std::mutex> lk(m_lock);
        not_empty.wait(lk, [this]{return !data.empty(); });
        int val = data.front();
        data.pop();
        not_full.notify_all();
        return val;
    }

    int size() {
        std::unique_lock<std::mutex> lk(m_lock);
        return data.size();
    }

private:
    int cap;
    std::mutex m_lock;
    std::condition_variable not_empty;
    std::condition_variable not_full;
    std::queue<int> data;
};


USI_NS_END