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
    vector<vector<char>> parent(n + 1, vector<char>(m + 1, ' '));

    for (int i = 1; i <= n; ++i) {
        dp[i][0] = dp[i-1][0] + cost_delete;
        parent[i][0] = 'D';
    }
    for (int j = 1; j <= m; ++j) {
        dp[0][j] = dp[0][j-1] + cost_insert;
        parent[0][j] = 'I';
    }

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int diag = dp[i-1][j-1] + (A[i-1] == B[j-1] ? 0 : cost_replace);
            int del = dp[i-1][j] + cost_delete;
            int ins = dp[i][j-1] + cost_insert;

            int min_cost = min({diag, del, ins});
            if (ins == min_cost) {
                dp[i][j] = ins;
                parent[i][j] = 'I';
            } else if (del == min_cost) {
                dp[i][j] = del;
                parent[i][j] = 'D';
            } else {
                dp[i][j] = diag;
                parent[i][j] = (A[i-1] == B[j-1]) ? 'M' : 'R';
            }
        }
    }

    vector<char> ops;
    int i = n, j = m;
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && (parent[i][j] == 'M' || parent[i][j] == 'R')) {
            ops.push_back(parent[i][j]);
            --i; --j;
        } else if (i > 0 && parent[i][j] == 'D') {
            ops.push_back('D');
            --i;
        } else if (j > 0 && parent[i][j] == 'I') {
            ops.push_back('I');
            --j;
        } else if (i > 0) {
            ops.push_back('D');
            --i;
        } else {
            ops.push_back('I');
            --j;
        }
    }
    reverse(ops.begin(), ops.end());

    for (char op : ops) cout << op;
    cout << '\n' << A << '\n' << B << '\n';

    return 0;
}