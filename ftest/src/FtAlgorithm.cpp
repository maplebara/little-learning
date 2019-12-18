#include "gtest/gtest.h"
#include "algorithm/StringMatcher.h"
#include "algorithm/LruCache.h"

using namespace usi;

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

TEST_F(FtAlgorithm, lru_cache_algo_test)
{
    LruCache cache(2);

    cache.put(1, 1);
    cache.put(2, 2);
    ASSERT_EQ(cache.get(1), 1);          // 返回  1
    cache.put(3, 3);                     // 该操作会使得key[2]作废
    ASSERT_EQ(cache.get(2), INVALID_U32);           // 返回无效值 (未找到)
    cache.put(4, 4);                     // 该操作会使得key[1]作废
    ASSERT_EQ(cache.get(1), INVALID_U32);                    // 返回无效值 (未找到)
    ASSERT_EQ(cache.get(3), 3);                    // 返回  3
    ASSERT_EQ(cache.get(4), 4);                    // 返回  4
}


