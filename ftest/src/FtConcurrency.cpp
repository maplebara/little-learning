#include "gtest/gtest.h"
#include "concurrent/Thread.h"
#include "FtConcurrentAction.h"
#include "concurrent/SpinLock.h"

using namespace usi;

class FtConcurrency : public ::testing::Test
                    , public FtConcurrentAction {
protected:
    void SetUp() override {
    }

    void TearDown() override {  }
};

TEST_F(FtConcurrency, class_Thread_Test)
{
    Thread thread(action);
    thread.join();
    printf("hello\n");
}

TEST_F(FtConcurrency, Spin_lock_Test)
{
    U32 num = 0;
    std::vector<Thread> threads;
    for(int i = 0; i < 10; ++i)
    {
        threads.emplace_back(assgin, &num);
    }
    
    for(int i = 0; i < 10; ++i)
    {
        threads[i].join();
    }
    printf("end!!\n");
}


