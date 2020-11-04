#pragma once 

#include <unistd.h>

pid_t GetTid();

void* s_memcpy(void* dest, const void* src, size_t n);

