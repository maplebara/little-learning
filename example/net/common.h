#pragma once

#include <netinet/in.h>
#include <unistd.h>

struct TestData
{
    char c;
    int32_t a;
};

struct TestData1
{
    char c;
    int32_t a;
}__attribute__ ((__packed__));

struct SessionMessage
{
    uint32_t length; 
    uint32_t number;
}__attribute__ ((__packed__));

struct PayLoad
{
    uint32_t length; 
    char payLoad[0];
};

int readAll(int fd, void* buf, size_t len);

int writeAll(int fd, void* buf, size_t len);


