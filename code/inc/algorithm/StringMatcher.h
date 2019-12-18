#include "basic/BasicMacro.h"
#include "basic/BasicType.h"
#include <string>
#include <vector>

using std::vector;
using std::string;

USI_NS_BEGIN

S32 match(const string& str, const string& pat)
{
    S32 n = str.size(), m = pat.size();
    for(S32 i = 0; i <= n - m; ++i)
    {
        S32 j = 0;
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
    KmpMatcher(const string& pat) : dfs(pat.size(), 0), pat(pat) {}

    S32 match(const string& text)
    {
        S32 matched = 0;
        for(S32 i = 0; i < text.size(); ++i)
        {
            while(matched > 0 && pat[matched] != text[i])
                matched = dfs[matched];
            
            if(pat[matched] == text[i])
                matched++;

            if(matched == pat.size())
                return i + 1 - matched;
        }
        return -1;
    }

    vector<S32> matchAll(const string& text)
    {
        vector<S32> ans;
        S32 matched = 0;
        for(S32 i = 0; i < text.size(); ++i)
        {
            while(matched > 0 && pat[matched] != text[i])
                matched = dfs[matched];
            
            if(pat[matched] == text[i])
                matched++;

            if(matched == pat.size())
                ans.push_back(i + 1 - matched);
        }
        return ans;
    }

private:
    void computeDfs()
    {
        S32 k = 0;
        for(S32 i = 1; i < pat.size(); ++i)
        {
            while(k > 0 && pat[k + 1] != pat[i])
                k = dfs[k];
            
            if(pat[k + 1] == pat[i])
                k++;
            
            dfs[i] = k;
        }
    }

private:
    vector<S32> dfs;
    string pat;
};

USI_NS_END
