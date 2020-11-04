#include "basic/util.h"
#include <sys/syscall.h>


pid_t GetTid()
{
    return syscall(SYS_gettid);
}

constexpr int WORD_LEN = 8;

void* s_memcpy(void* dest, const void* src, size_t n)
{
    auto s_data = (const char*)src;
    auto d_data = (char*)dest;
    while(n >= 8) {
        auto ls_data = (const long*)s_data;
        auto ld_data = (long*)d_data;
        *ld_data = *ls_data;
        s_data += 8, d_data += 8;
        n -= 8;
    }
    if(n >= 4) {
        auto is_data = (const int*)s_data;
        auto id_data = (int*)d_data;
        *id_data = *is_data;
        s_data += 4, d_data += 4;
        n -= 4;
    }
    while(n--) {
        *d_data = *s_data;
        ++d_data, ++s_data;
    }
    return dest;
}

