#pragma once

#include <pthread.h>
#include <atomic>

#include "basic/BasicMacro.h"

USI_NS_BEGIN

struct SpinLock
{
    SpinLock()
    {
        pthread_spin_init(&m_lock, PTHREAD_PROCESS_PRIVATE);
        lock();
    }

    ~SpinLock()
    {
        unlock();
        pthread_spin_destroy(&m_lock);
    }

    void lock()
    {
        pthread_spin_lock(&m_lock);
    }

    void unlock()
    {
        pthread_spin_unlock(&m_lock);
    }

private:
    pthread_spinlock_t m_lock;
};

struct CasLock
{
    CasLock ()
    {
        m_lock.clear();
    }

    void lock()
    {
        while(m_lock.test_and_set(std::memory_order_acquire)) ;
    }

    void unlock()
    {
        m_lock.clear(std::memory_order_release);
    }

private:
    std::atomic_flag m_lock;
};

// template<typename T>
// struct RawSpinLock
// {
//     void lock()
//     {
//         m_lock.lock();
//     }

//     void unlock()
//     {
//         m_lock.unlock();
//     }

// private:
//     T m_lock;
// };

// using SpinLock = RawSpinLock

USI_NS_END


