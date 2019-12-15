#include "BasicMacro.h"
#include <string>
#include <vector>

using std::vector;
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
    KmpMatcher(const string& pat) : dfs(pat.size(), 0), pat(pat) {}

    int match(const string& text)
    {
        int matched = 0;
        for(int i = 0; i < text.size(); ++i)
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

    vector<int> matchAll(const string& text)
    {
        vector<int> ans;
        int matched = 0;
        for(int i = 0; i < text.size(); ++i)
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
        int k = 0;
        for(int i = 1; i < pat.size(); ++i)
        {
            while(k > 0 && pat[k + 1] != pat[i])
                k = dfs[k];
            
            if(pat[k + 1] = pat[i])
                k++;
            
            dfs[i] = k;
        }
    }

private:
    vector<int> dfs;
    string pat;
};

USI_NS_END
