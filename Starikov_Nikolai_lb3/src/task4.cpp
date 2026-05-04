#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <climits>

using namespace std;

int main() {
    int cost_replace, cost_insert, cost_delete, cost_replace1to2;
    cin >> cost_replace >> cost_insert >> cost_delete >> cost_replace1to2;
    
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
            int del = dp[i-1][j] + cost_delete;
            int ins = dp[i][j-1] + cost_insert;
            int rep = dp[i-1][j-1] + (A[i-1] == B[j-1] ? 0 : cost_replace);
            int rep2 = (j >= 2) ? dp[i-1][j-2] + cost_replace1to2 : INT_MAX;
            
            int best = del;
            char bestOp = 'D';
            if (ins < best) { best = ins; bestOp = 'I'; }
            if (rep < best) { best = rep; bestOp = 'R'; }
            if (rep2 < best) { best = rep2; bestOp = '2'; }
            
            dp[i][j] = best;
            parent[i][j] = bestOp;
        }
    }
    
    const int W = 4;
    
    cout << "\n=== DP таблица (стоимости) ===\n";
    cout << setw(W) << " ";
    for (int j = 0; j <= m; ++j) {
        if (j == 0) cout << setw(W) << "   ε";
        else cout << setw(W) << B[j-1];
    }
    cout << "\n";
    for (int i = 0; i <= n; ++i) {
        if (i == 0) cout << setw(W) << "   ε";
        else cout << setw(W) << A[i-1];
        for (int j = 0; j <= m; ++j) {
            cout << setw(W) << dp[i][j];
        }
        cout << "\n";
    }
    
    cout << "\n=== Таблица операций (D/I/R/2) ===\n";
    cout << setw(W) << "  ";
    for (int j = 0; j <= m; ++j) {
        if (j == 0) cout << setw(W) << "   ε";
        else cout << setw(W) << B[j-1];
    }
    cout << "\n";
    for (int i = 0; i <= n; ++i) {
        if (i == 0) cout << setw(W) << "   ε";
        else cout << setw(W) << A[i-1];
        for (int j = 0; j <= m; ++j) {
            char c = parent[i][j];
            if (c == ' ') cout << setw(W) << "  .";
            else cout << setw(W) << c;
        }
        cout << "\n";
    }
    
    cout << "\nЛегенда:\n";
    cout << " D – удалить символ из A\n";
    cout << " I – вставить символ из B\n";
    cout << " R – заменить символ A на символ B (1→1)\n";
    cout << " 2 – заменить один символ A на два символа B (1→2)\n";
    
    vector<string> steps;
    int i = n, j = m;
    while (i > 0 || j > 0) {
        char op = parent[i][j];
        if (op == 'D') {
            steps.push_back("Удалить '" + string(1, A[i-1]) + "'");
            i--;
        } else if (op == 'I') {
            steps.push_back("Вставить '" + string(1, B[j-1]) + "'");
            j--;
        } else if (op == 'R') {
            steps.push_back("Заменить '" + string(1, A[i-1]) + "' на '" + string(1, B[j-1]) + "'");
            i--; j--;
        } else if (op == '2') {
            steps.push_back("Заменить '" + string(1, A[i-1]) + "' на \"" + string(1, B[j-2]) + string(1, B[j-1]) + "\"");
            i--; j -= 2;
        }
    }
    reverse(steps.begin(), steps.end());
    
    cout << "\n=== Последовательность операций ===\n";
    for (size_t k = 0; k < steps.size(); ++k)
        cout << k+1 << ". " << steps[k] << "\n";
    
    cout << "\n=== Минимальная стоимость = " << dp[n][m] << " ===\n";
    
    return 0;
}