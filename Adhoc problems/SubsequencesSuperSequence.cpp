#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class SequenceSolver {
public:
    static int lcs_length(const std::string& s1, const std::string& s2) 
{
        int m = s1.length();
        int n = s2.length();
        
        if (m == 0 || n == 0) return 0;
        
        std::vector<int> prev(n + 1, 0);
        std::vector<int> curr(n + 1, 0);
        
        for (int i = 1; i <= m; ++i) 
        {
            for (int j = 1; j <= n; ++j) 
            {
                if (s1[i - 1] == s2[j - 1]) 
                {
                    curr[j] = prev[j - 1] + 1;
                } 
                else 
                {
                    curr[j] = std::max(prev[j], curr[j - 1]);
                }
            }
            std::swap(prev, curr);
        }
        
        return prev[n];
    }
    
    static std::string lcs_string(const std::string& s1, const std::string& s2) 
{
        int m = s1.length();
        int n = s2.length();
        
        if (m == 0 || n == 0) return "";
        
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
        
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        
        std::string lcs;
        lcs.reserve(dp[m][n]);
        int i = m, j = n;
        
        while (i > 0 && j > 0) 
        {
            if (s1[i - 1] == s2[j - 1]) 
            {
                lcs += s1[i - 1];
                --i;
                --j;
            } 
            else if (dp[i - 1][j] > dp[i][j - 1]) 
            {
                --i;
            } 
            else 
            {
                --j;
            }
        }
        
        std::reverse(lcs.begin(), lcs.end());
        return lcs;
    }
    
    static int scs_length(const std::string& s1, const std::string& s2) 
{
        return s1.length() + s2.length() - lcs_length(s1, s2);
    }
    
    static std::string scs_string(const std::string& s1, const std::string& s2) 
{
        int m = s1.length();
        int n = s2.length();
        
        if (m == 0) return s2;
        if (n == 0) return s1;
        
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
        
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        
        std::string scs;
        scs.reserve(m + n);
        int i = m, j = n;
        
        while (i > 0 && j > 0) 
        {
            if (s1[i - 1] == s2[j - 1]) 
            {
                scs += s1[i - 1];
                --i;
                --j;
            } 
            else if (dp[i - 1][j] > dp[i][j - 1]) 
            {
                scs += s1[i - 1];
                --i;
            } 
            else 
            {
                scs += s2[j - 1];
                --j;
            }
        }
        
        while (i > 0) 
        {
            scs += s1[i - 1];
            --i;
        }
        
        while (j > 0) 
        {
            scs += s2[j - 1];
            --j;
        }
        
        std::reverse(scs.begin(), scs.end());
        return scs;
    }
};

int main() 
{
    std::string s1 = "AGGTAB";
    std::string s2 = "GXTXAYB";
    
    std::cout << "String 1: " << s1 << std::endl;
    std::cout << "String 2: " << s2 << std::endl;
    std::cout << std::endl;
    
    std::cout << "LCS Length: " << SequenceSolver::lcs_length(s1, s2) << std::endl;
    std::cout << "LCS String: " << SequenceSolver::lcs_string(s1, s2) << std::endl;
    std::cout << std::endl;
    
    std::cout << "SCS Length: " << SequenceSolver::scs_length(s1, s2) << std::endl;
    std::cout << "SCS String: " << SequenceSolver::scs_string(s1, s2) << std::endl;
    std::cout << std::endl;
    
    std::string s3 = "abcd";
    std::string s4 = "xycd";
    
    std::cout << "String 3: " << s3 << std::endl;
    std::cout << "String 4: " << s4 << std::endl;
    std::cout << std::endl;
    
    std::cout << "LCS Length: " << SequenceSolver::lcs_length(s3, s4) << std::endl;
    std::cout << "LCS String: " << SequenceSolver::lcs_string(s3, s4) << std::endl;
    std::cout << std::endl;
    
    std::cout << "SCS Length: " << SequenceSolver::scs_length(s3, s4) << std::endl;
    std::cout << "SCS String: " << SequenceSolver::scs_string(s3, s4) << std::endl;
    
    return 0;
}
