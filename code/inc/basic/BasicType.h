#pragma once 

#include "basic/BasicMacro.h"

USI_NS_BEGIN

typedef    signed char         int8_t;
typedef    signed short        int16_t;
typedef    signed long         int32_t;
typedef    signed long long    int64_t;

typedef    unsigned char       uint8_t;
typedef    unsigned short      uint16_t;
typedef    unsigned long       uint32_t;
typedef    unsigned long long  uint64_t;


typedef    int8_t     S8;
typedef    int16_t    S16;
typedef    int32_t    S32;
typedef    int64_t    S64;

typedef    uint8_t    U8;
typedef    uint16_t   U16;
typedef    uint32_t   U32;
typedef    uint64_t   U64;

const U32 INVALID_U32 = 0xffffffff;

template<typename T>
class IsBaseType
{
public:
    static bool isBaseType(){return YES;}
private:
    enum {YES = 0, NO = 1};
};

#define IS_BASE_TYPE(type) template<>       \
class IsBaseType<type>                      \
{                                           \
public:                                     \
    static bool isBaseType(){return YES;}    \
private:                                    \
    enum {YES = 1, NO = 0};          \
};

IS_BASE_TYPE(U8)
IS_BASE_TYPE(S8)
IS_BASE_TYPE(U16)
IS_BASE_TYPE(S16)
IS_BASE_TYPE(U32)
IS_BASE_TYPE(S32)
IS_BASE_TYPE(U64)
IS_BASE_TYPE(S64)

USI_NS_END
