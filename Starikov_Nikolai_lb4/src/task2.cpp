#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    string A, B;
    getline(cin, A);
    getline(cin, B);

    if (A.size() != B.size()) {
        cout << -1 << endl;
        return 0;
    }
    int n = A.size();
    if (n == 0) {
        cout << 0 << endl;
        return 0;
    }

    cout << "Префикс-функция для B=\"" << B << "\":\n";
    vector<int> pi(n, 0);
    for (int i = 1; i < n; ++i) {
        cout << "i=" << i << " B[i]=" << B[i] << "\n";
        int j = pi[i-1];
        cout << "  j=" << j << "\n";
        while (j > 0 && B[i] != B[j]) {
            cout << "  сравнение B[i]=" << B[i] << " и B[" << j << "]=" << B[j] << " не равно, j=pi[" << j-1 << "]=" << pi[j-1] << "\n";
            j = pi[j-1];
        }
        if (B[i] == B[j]) {
            cout << "  сравнение B[i]=" << B[i] << " и B[" << j << "]=" << B[j] << " равно, j++ -> " << j+1 << "\n";
            j++;
        } else {
            cout << "  сравнение B[i]=" << B[i] << " и B[" << j << "]=" << B[j] << " не равно, j=0\n";
        }
        pi[i] = j;
        cout << "  pi[" << i << "]=" << j << "\n";
    }
    cout << "Префикс-функция: ";
    for (int v : pi) cout << v << " ";
    cout << "\n\nПоиск B в A+A (виртуально):\n";

    int j = 0;
    for (int i = 0; i < 2 * n; ++i) {
        char cur = A[i % n];
        cout << "i=" << i << " (idx mod n=" << i%n << ") cur='" << cur << "', j=" << j << "\n";
        while (j > 0 && cur != B[j]) {
            cout << "  сравнение cur='" << cur << "' и B[" << j << "]='" << B[j] << "' не равно, j=pi[" << j-1 << "]=" << pi[j-1] << "\n";
            j = pi[j-1];
        }
        if (cur == B[j]) {
            cout << "  сравнение cur='" << cur << "' и B[" << j << "]='" << B[j] << "' равно, j++ -> " << j+1 << "\n";
            j++;
        } else {
            cout << "  сравнение cur='" << cur << "' и B[" << j << "]='" << B[j] << "' не равно, j остаётся " << j << "\n";
        }
        if (j == n) {
            int pos = i - n + 1;
            cout << "  *** полное совпадение, pos=" << pos << "\n";
            if (pos < n) {
                cout << pos << endl;
                return 0;
            } else {
                cout << "  pos >= n, пропускаем, j=pi[" << n-1 << "]=" << pi[n-1] << "\n";
                j = pi[j-1];
            }
        }
    }
    cout << -1 << endl;
    return 0;
}