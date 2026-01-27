#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n = 5, m = 5; // grid size
    vector<vector<int>> dp(n, vector<int>(m, 0));

    // Example: update dp[i][j] based on 4 directions
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // check UP
            if (i - 1 >= 0) {
                dp[i][j] += dp[i - 1][j];
            }
            // check DOWN
            if (i + 1 < n) {
                dp[i][j] += dp[i + 1][j];
            }
            // check LEFT
            if (j - 1 >= 0) {
                dp[i][j] += dp[i][j - 1];
            }
            // check RIGHT
            if (j + 1 < m) {
                dp[i][j] += dp[i][j + 1];
            }
        }
    }

    for (const auto& row : dp) {
        for (const auto& val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    return 0;
}