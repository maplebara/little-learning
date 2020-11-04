#include "gtest/gtest.h"
#include "concurrent/Thread.h"
#include "FtConcurrentAction.h"
#include "concurrent/SpinLock.h"
#include "concurrent/ThreadLocal.h"
#include "concurrent/BlockingQueue.h"
#include <thread>

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

TEST_F(FtConcurrency, DISABLED_blockingQueue_test)
{
    BlockingQueue bq(10);
    int a = 10, b = 33;
    auto cf = [&bq](int val) { 
        for(int i = 0; i < 10; ++i) {
            bq.put(val + i);
        }    
    };
    auto pf = [&bq]() { 
        while(true) {
            printf("tid[%x] consumer elem %d\n", std::this_thread::get_id(), bq.pop());
        }
     };
    std::thread producer1(cf, a);
    std::thread producer2(cf, b);
    std::thread consumer1(pf);
    std::thread consumer2(pf);

    producer1.join();
    producer2.join();
    consumer1.join();
    consumer2.join();
}


