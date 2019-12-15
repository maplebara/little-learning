#include "BasicMacro.h"
#include <string>

using std::string;

USI_NS_BEGIN

int match(const string& str, const string& pat)
{
    int n = str.size(), m = pat.size();
    for(int i = 0; i <= n - m; ++i)
    {
        int j = 0;
        while(j < m)
        {
            if (str[i + j] != pat[j]) break;
            j++;
        }
        if (j == m) return i;
    }

    return -1;
}

struct KmpMatcher
{

private:

};

USI_NS_END
