#include "concurrent/Thread.h"
#include <system_error>
#include <cerrno>

USI_NS_BEGIN

namespace {
    thread_local Thread* self_thread = nullptr;

}

Thread::Thread(std::function<void()> exec) : exec(exec)
{
    pthread_attr_init(&attr);
    int ret = pthread_create(&tid, &attr, &Thread::run, this);
    if(ret)
    {
        throw std::system_error(std::error_code(), "pthread_create error");
    }
    printf("create\n");
}

Thread::~Thread()
{
    pthread_detach(tid);
    pthread_attr_destroy(&attr);
    printf("ruin\n");
}

void* Thread::run(void* args)
{
    Thread* thread = (Thread*)args;
    self_thread = thread;

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
    pthread_join(tid, nullptr);
}

void Thread::detach()
{ 
    if(isDetached()) return ;
    pthread_detach(tid);
}

bool Thread::isDetached() const
{
    int state = 0;

    pthread_attr_getdetachstate(&attr, &state);

    return state == PTHREAD_CREATE_DETACHED;
}

USI_NS_END