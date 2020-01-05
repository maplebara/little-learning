#include "gtest/gtest.h"
#include "net/common.h"
#include <boost/lexical_cast.hpp>


class FtBasic : public ::testing::Test {
protected:
    void SetUp() override {
    }
    void TearDown() override {  }
};


TEST_F(FtBasic, struct_memory_assign)
{
    ASSERT_EQ(sizeof(TestData), 8);
    ASSERT_EQ(sizeof(TestData1), 5);
    ASSERT_EQ(sizeof(TestData2), 1);
    ASSERT_EQ(sizeof(TestData3), 16);
    ASSERT_EQ(sizeof(TestData4), 1);
    ASSERT_EQ(sizeof(SessionMessage), 8);
}

TEST_F(FtBasic, boost_cast_test)
{
    using boost::lexical_cast;
    int a = lexical_cast<int>("123");
    double b = lexical_cast<double>("123.12");
    ASSERT_EQ(a, 123);
    ASSERT_EQ(b, 123.12);
}