#pragma once

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

struct TestData2
{
    char c;
};

struct TestData3
{
    char c;
    long l;
};

struct TestData4
{
    char b;
    char c[0];
};

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

