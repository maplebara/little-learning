#pragma once

#include <pthread.h>
#include "basic/BasicMacro.h"

USI_NS_BEGIN

template<typename T>
struct ThreadLocal
{
    ThreadLocal()
    {
        pthread_key_create(&value, &ThreadLocal::destroy);
    }

    ~ThreadLocal()
    {
        pthread_key_delete(value);
    }

    T& get()
    {
        T* temp = (T*)pthread_getspecific(value);
        if(!temp)
        {
            temp = new T();
            pthread_setspecific(value, temp);
        }
        return *temp;
    }

private:
    static void destroy(void* data)
    {
        T* ptr = static_cast<T*>(data);
        typedef char T_MUST_BE_COMPLETE_TYPE[sizeof(T) == 0 ? -1 : 1];
        T_MUST_BE_COMPLETE_TYPE dummy; (void)dummy;
        delete ptr;
    }


private:
    pthread_key_t value;
};

USI_NS_END

