#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    int cost_replace, cost_insert, cost_delete;
    cin >> cost_replace >> cost_insert >> cost_delete;
    
    string A, B;
    cin >> A >> B;
    
    int n = A.size();
    int m = B.size();
    
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    
    for (int i = 1; i <= n; ++i) {
        dp[i][0] = dp[i-1][0] + cost_delete;
    }
    for (int j = 1; j <= m; ++j) {
        dp[0][j] = dp[0][j-1] + cost_insert;
    }
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (A[i-1] == B[j-1]) {
                dp[i][j] = dp[i-1][j-1];
            } else {
                int replace_cost = dp[i-1][j-1] + cost_replace;
                int delete_cost  = dp[i-1][j] + cost_delete;
                int insert_cost  = dp[i][j-1] + cost_insert;
                dp[i][j] = min({replace_cost, delete_cost, insert_cost});
            }
        }
    }
    
    cout << dp[n][m] << endl;
    
    return 0;
}