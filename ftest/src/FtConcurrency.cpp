#include "gtest/gtest.h"
#include "concurrent/Thread.h"
#include "FtConcurrentAction.h"
#include "concurrent/SpinLock.h"
#include "concurrent/ThreadLocal.h"

using namespace usi;
ThreadLocal<int> tsd;

namespace {

void func1(void* num)
{
    int* a = (int*)num;
    int& temp = tsd.get();
    temp = *a;
}

}

class FtConcurrency : public ::testing::Test
                    , public FtConcurrentAction {
protected:
    void SetUp() override {
    }

    void TearDown() override {  }
};

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
    ASSERT_EQ(num, 100 * 1000);
}

TEST_F(FtConcurrency, Cas_lock_Test)
{
    U32 num = 0;
    std::vector<Thread> threads;
    for(int i = 0; i < 10; ++i)
    {
        threads.emplace_back(assginCas, &num);
    }
    
    for(int i = 0; i < 10; ++i)
    {
        threads[i].join();
    }
    ASSERT_EQ(num, 100 * 1000);
}

TEST_F(FtConcurrency, thread_local_data_)
{
    int a = 10;
    int b = 20;
    ThreadLocal<int> tsd;
    Thread thread1(func1, &a);
    Thread thread2(func1, &b);
    thread1.join();
    thread2.join();
}


