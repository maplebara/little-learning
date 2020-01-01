#ifndef __LITTLE_LEARNING_FTEST_FTCONCURRENTaCTION_H__
#define __LITTLE_LEARNING_FTEST_FTCONCURRENTaCTION_H__

#include <stdio.h>
#include <thread>
#include <chrono>
#include <pthread.h>

#include "basic/util.h"
#include "basic/BasicType.h"
#include "concurrent/SpinLock.h"

using namespace usi;

static SpinLock spinlock;
static CasLock casLock;

struct FtConcurrentAction
{
    static void assgin(void* count)
    {
        U32* ptr = (U32*)count;
        spinlock.lock();
        for(int i = 0; i < 10000; ++i)
        {
            ++(*ptr);
        }
        spinlock.unlock();
    }

    static void assginCas(void* count)
    {
        U32* ptr = (U32*)count;
        casLock.lock();
        for(int i = 0; i < 10000; ++i)
        {
            ++(*ptr);
        }
        casLock.unlock();
    }
};


#endif
