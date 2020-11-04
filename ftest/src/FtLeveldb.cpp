#include "gtest/gtest.h"
#include <leveldb/db.h>
#include "net.h"

class FtLevelDb : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {  }
};


TEST_F(FtLevelDb, database_open_seccess_test)
{
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
    ASSERT_TRUE(status.ok());
}

TEST_F(FtLevelDb, database_net_seccess_test)
{
    eventLoop();
}





