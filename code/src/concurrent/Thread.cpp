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
}

Thread::~Thread()
{
    pthread_detach(tid);
    pthread_attr_destroy(&attr);
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

}

USI_NS_END