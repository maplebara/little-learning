#include <stdio.h>

#define USI_ASSERT_TRUE(expr) \
        do {  \
            if(!(expr))  \
            {          \
                printf("The expr (%s)[%d] is failed!!\n", __FILE__, __LINE__); \
                return -1;   \
            } \
        } while(0)



//////////////////////////

#define USI_NS_BEGIN namespace {
#define USI_NS_END }



