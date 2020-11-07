#pragma once

#include "leveldb/status.h"
#include <string>
#include <unistd.h>

using leveldb::Status;

struct AbstractDbResponse
{
    virtual ~AbstractDbResponse() {}

    virtual int response(int fd) = 0;
    virtual void destroy() = 0;
};

struct DbResponse : AbstractDbResponse
{
    DbResponse(Status stat = Status()) : stat(stat) {}

    virtual int response(int fd) override
    {
        int res = ::write(fd, stat.ToString().c_str(), stat.ToString().size());
        return res;
    }

    void destroy() override {}

private:
    Status stat;
};

struct GetDbResponse : AbstractDbResponse
{
    virtual int response(int fd) override
    {
        int res = ::write(fd, rsp.c_str(), rsp.size());
        if(res < 0) return res;
        res = ::write(fd, stat.ToString().c_str(), stat.ToString().size());
        return res;
    }

    void destroy() override {
        ~GetDbResponse();
    }

    ~GetDbResponse() {
        delete this;
    }

    std::string* getRsp() {
        return &rsp;
    }

    void set(const Status& stat)
    {
        this->stat = stat;
    }

private:
    Status stat;    
    std::string rsp; 
};





