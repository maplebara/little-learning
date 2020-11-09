#pragma once

#include "ProducerConsumerQueue.h"
#include "concurrent/Singleton.h"
#include <mutex>
#include <condition_variable>
#include <utility>
#include "comm_struct.h"

constexpr uint32_t MAX_BUF = 1024 * 1024;

template<uint32_t MAX_BUF>
struct TaskQueue : usi::Singleton<TaskQueue<MAX_BUF>>
{
    TaskQueue() : pq(MAX_BUF) {}

    void write(Task&& task) {
        if(!pq.write(task)) {
            std::unique_lock<std::mutex> lk(mt);
            not_empty.wait(lk, [task, this]{return pq.write(task); });
            not_full.notify_all();
        }
    }

    void read(Task& record) {
        if(!pq.read(record)) {
            std::unique_lock<std::mutex> lk(mt);
            not_full.wait(lk, [&record, this]{return pq.read(record);});
            not_empty.notify_all();
        }
    }

private:
    std::mutex mt;
    std::condition_variable not_empty;
    std::condition_variable not_full;
    folly::ProducerConsumerQueue<Task> pq;
};

using TaskQueues = TaskQueue<MAX_BUF>;











