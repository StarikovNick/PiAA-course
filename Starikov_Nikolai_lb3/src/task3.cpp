#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    string S, T;
    cin >> S >> T;
    int n = S.size();
    int m = T.size();

    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 1; i <= n; ++i) {
        dp[i][0] = i;
    }
    for (int j = 1; j <= m; ++j) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (S[i-1] == T[j-1]) {
                dp[i][j] = dp[i-1][j-1];
            } else {
                int replace_cost = dp[i-1][j-1] + 1;
                int delete_cost = dp[i-1][j] + 1;
                int insert_cost = dp[i][j-1] + 1;
                dp[i][j] = min({replace_cost, delete_cost, insert_cost});
            }
        }
    }

    cout << dp[n][m] << endl;

    return 0;
}