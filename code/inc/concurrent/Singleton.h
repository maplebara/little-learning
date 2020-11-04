#pragma once

#include <pthread.h>
#include "basic/BasicMacro.h"
#include "basic/UnCopyable.h"

USI_NS_BEGIN

template<typename T>
struct Singleton : UnCopyable
{
    static T& getInstance()
    {
        pthread_once(&pOnce, &Singleton::init);
        return *pInstance;
    }

private:
    static void init()
    {
        pInstance = new T();
    }

private:
    static T* pInstance;
    static pthread_once_t pOnce;
};

template<typename T>
T* Singleton<T>::pInstance = nullptr;

template<typename T>
pthread_once_t Singleton<T>::pOnce = PTHREAD_ONCE_INIT;

//It's thread safe in C++11 by using local static variable to implement Singleton Pattern

USI_NS_END


