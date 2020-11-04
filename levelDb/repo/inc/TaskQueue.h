#pragma once

#include "ProducerConsumerQueue.h"
#include "concurrent/Singleton.h"

constexpr uint32_t MAX_BUF = 1024 * 1024;

template<uint32_t MAX_BUF>
struct TaskQueue : usi::Singleton<TaskQueue<MAX_BUF>>
{
    TaskQueue() : pq(MAX_BUF) {}

    template <class... Args>
    bool write(Args&&... recordArgs) {
        return pq.write(std::forward<Args>(recordArgs)...);
    }

    bool read(Task& record) {
        return pq.read(record);
    }

private:
    folly::ProducerConsumerQueue<Task> pq;
};

using TaskQueues = TaskQueue<MAX_BUF>;











