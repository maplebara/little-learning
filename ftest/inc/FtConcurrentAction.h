#ifndef __LITTLE_LEARNING_FTEST_FTCONCURRENTaCTION_H__
#define __LITTLE_LEARNING_FTEST_FTCONCURRENTaCTION_H__

#include <stdio.h>
#include <thread>
#include <chrono>

struct FtConcurrentAction
{
    static void action()
    {
        printf("The thread is starting.\n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        printf("The thread end.\n");
    }
};


#endif
