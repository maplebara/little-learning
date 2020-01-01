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


