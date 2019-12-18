#include "gtest/gtest.h"
#include "concurrent/Thread.h"
#include "FtConcurrentAction.h"

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


