#include "concurrent/Thread.h"
#include <system_error>
#include <cerrno>
#include "basic/util.h"

USI_NS_BEGIN

namespace {
    thread_local Thread* self_thread = nullptr;

}

Thread::Thread(std::function<void()> exec) : exec(exec)
{
    pthread_attr_init(&attr);
    sem_init(&semaphore, PTHREAD_PROCESS_PRIVATE, 0);
    int ret = pthread_create(&threadId, &attr, &Thread::run, this);
    if(ret)
    {
        throw std::system_error(std::error_code(), "pthread_create error");
    }
    sem_wait(&semaphore);
    printf("create\n");
}

Thread::~Thread()
{
    pthread_detach(threadId);
    pthread_attr_destroy(&attr);
    sem_destroy(&semaphore);
    printf("ruin\n");
}

void* Thread::run(void* args)
{
    Thread* thread = (Thread*)args;
    self_thread = thread;

    thread->tid = GetTid();

    sem_post(&(thread->semaphore));
    thread->exec();

    return 0;
}

void Thread::join()
{
    if(isDetached()) 
    {
        printf("The thread is detached!!\n");
        return ;
    }
    pthread_join(threadId, nullptr);
}

void Thread::detach()
{ 
    if(isDetached()) return ;
    pthread_detach(threadId);
}

bool Thread::isDetached() const
{
    int state = 0;

    pthread_attr_getdetachstate(&attr, &state);

    return state == PTHREAD_CREATE_DETACHED;
}

USI_NS_END