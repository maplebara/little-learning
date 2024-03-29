#include "gtest/gtest.h"
#include "net/common.h"
#include <boost/lexical_cast.hpp>
#include <string>
#include "basic/util.h"

using std::string;

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

struct A
{
    static int a;
    const int c = 0;
    int b;
};

int A::a = 9;

TEST_F(FtBasic, std_string_test)
{
    A a;
    printf("size=%lu\n", sizeof(A));
}

TEST_F(FtBasic, s_memcpy_test)
{
    char src[] = "abc";
    char dst[100];
    char s_dst[100];
    memset(dst, 0x00, 100);
    memset(s_dst, 0x00, 100);
    memcpy(dst, src, sizeof src);
    s_memcpy(s_dst, src, sizeof src);
    string s_str(dst, sizeof src);
    string ss_str(s_dst, sizeof src);
    printf("s_str=%s\n", s_str.c_str());
    printf("ss_str=%s\n", ss_str.c_str());
    ASSERT_EQ(s_str, ss_str);
}



