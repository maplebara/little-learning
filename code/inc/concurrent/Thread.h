#ifndef __LITTLE_LEARNING_CONCURRENT_THREAD_H__
#define __LITTLE_LEARNING_CONCURRENT_THREAD_H__

#include "basic/BasicMacro.h"
#include <pthread.h>
#include "functional"
#include "basic/UnCopyable.h"
#include <semaphore.h>

USI_NS_BEGIN

pid_t GetThreadId();

struct Thread : UnCopyable
{
    Thread(std::function<void()> exec);
    Thread(std::function<void(void* args)>, void* args);

    Thread(Thread&& other) noexcept;
    ~Thread();

    void join();
    void detach();

    bool isDetached() const;

    pthread_t getThreadId() const { return threadId; }
    pid_t  getTid() const { return tid; } 

    static Thread* GetThis();

private:
    static void* run(void*);

private:
    pid_t tid{-1};            //posix线程库的线程实现为一个轻量级进程(LWP)，其具有一个真实的进程ID，通常被称为tid。
    pthread_t threadId{0};    //由posix线程库维护的线程ID，不同进程中线程ID可能重复
    pthread_attr_t attr;
    sem_t semaphore;
    std::function<void()> exec;
    std::function<void(void*)> func;
    void* args{nullptr};
};

USI_NS_END

#endif
