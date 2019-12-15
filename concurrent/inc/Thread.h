#ifndef __LITTLE_LEARNING_CONCURRENT_THREAD_H__
#define __LITTLE_LEARNING_CONCURRENT_THREAD_H__

#include "BasicMacro.h"
#include <pthread.h>
#include "functional"
#include "UnCopyable.h"

USI_NS_BEGIN

struct Thread : UnCopyable
{
    Thread(std::function<void()> exec) : exec(exec) {}
    ~Thread() {}

    void start();

    pthread_t getTid() const { return tid; }

private:
    pthread_t tid{0};
    pthread_attr_t attr;

    std::function<void()> exec;
};

USI_NS_END

#endif
