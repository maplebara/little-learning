#pragma once

#include "gtest/gtest.h"

template<typename T>
struct Print
{
    const int Value = 1 / (sizeof(T) - sizeof(T));
};

template<int N>
struct IntValue
{
    enum { Value = N };
    using Result = IntValue<N>;
};

template<bool V> struct BoolType;

template<>
struct BoolType<false>
{
    enum { Value = false };
    using Result = BoolType<false>;
};

template<>
struct BoolType<true>
{
    enum { Value = true };
    using Result = BoolType<true>;
};

using FalseType = BoolType<false>;
using TrueType = BoolType<true>;

template<typename M, typename N>
struct IsEqual
{
    using Result = FalseType;
};

template<typename T>
struct IsEqual<T, T>
{
    using Result = TrueType;
};

template<int N> 
struct Factorial
{
    enum { Value = N * Factorial<N - 1>::Value };
};

template<>
struct Factorial<1>
{
    enum { Value = 1 };
};

template<>
struct Factorial<0>
{
    enum { Value = 1 };
};

class FtTemplate : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {  }
};


TEST_F(FtTemplate, struct_memory_assign)
{
    Print<IsEqual<IntValue<5>, IntValue<5>>::Result> p;
    // Print<Factorial<5>> pp;
    printf("fib(5)=%d\n", Factorial<5>::Value);
}



