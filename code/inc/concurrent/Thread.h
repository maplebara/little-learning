#ifndef __LITTLE_LEARNING_CONCURRENT_THREAD_H__
#define __LITTLE_LEARNING_CONCURRENT_THREAD_H__

#include "basic/BasicMacro.h"
#include <pthread.h>
#include "functional"
#include "basic/UnCopyable.h"

USI_NS_BEGIN

struct Thread : UnCopyable
{
    Thread(std::function<void()> exec);
    ~Thread();

    void join();

    pthread_t getTid() const { return tid; }

private:
    static void* run(void*);

private:
    pthread_t tid{0};
    pthread_attr_t attr;

    std::function<void()> exec;
};

USI_NS_END

#endif
