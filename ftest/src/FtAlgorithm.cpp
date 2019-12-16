#include "gtest/gtest.h"
#include "algorithm/StringMatcher.h"

class FtAlgorithm : public ::testing::Test {
 protected:
    void SetUp() override {
    }

    void TearDown() override {  }
};


TEST_F(FtAlgorithm, str_algo_kmp_matcher)
{
    KmpMatcher kmp("cssx");
    ASSERT_EQ(kmp.match("abcsssdab"), -1);
    ASSERT_EQ(kmp.match("abcssxsdab"), 2);
    ASSERT_EQ(kmp.match("abcssxxsdab"), 2);
}


