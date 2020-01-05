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

struct SessionMessage
{
    uint32_t length; 
    uint32_t number;
}__attribute__ ((__packed__));