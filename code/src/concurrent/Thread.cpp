#include "Thread.h"
#include <system_error>
#include <cerrno>

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
    pthread_attr_destroy(&attr);
}

void Thread::start()
{

}