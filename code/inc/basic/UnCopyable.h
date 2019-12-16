#ifndef __LITTLE_LEARNING_BASIC_UNCOPYABLE_H__
#define __LITTLE_LEARNING_BASIC_UNCOPYABLE_H__

#include "BasicMacro.h"

USI_NS_BEGIN

struct UnCopyable
{
    UnCopyable() {}

    UnCopyable(const UnCopyable&) = delete;
    UnCopyable& operator=(const UnCopyable&) = delete;
};

USI_NS_END

#endif