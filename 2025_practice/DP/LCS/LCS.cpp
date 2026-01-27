#include<iostream>
#include <vector>
#include <functional>

using namespace std;


std::function<int(const std::string&, const std::string& )> lcs = 
    [&](const std::string& s1, const std::string& s2) {
    int n = s1.size();
    int m = s2.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[n][m];
};



int main(int argc, char** argv)
{
    string s1 = "AGGTAB";
    string s2 = "GXTXAYB";
    cout << lcs(s1, s2) << endl;
    return (0);
}