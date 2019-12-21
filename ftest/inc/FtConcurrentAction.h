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

struct FtConcurrentAction
{
    static void action()
    {
        printf("The thread is starting.\n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        printf("The thread end.\n");
    }

    static void assgin(void* count)
    {
        U32* ptr = (U32*)count;
        SpinLock lock;
        for(int i = 0; i < 10000; ++i)
        {
            ++(*ptr);
        }
        printf("thread[%u] update count %lu!!\n", GetTid(), *ptr);
    }
};


#endif
