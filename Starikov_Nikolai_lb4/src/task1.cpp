#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    string P, T;
    getline(cin, P);
    getline(cin, T);

    int m = P.size();
    if (m == 0) {
        cout << -1 << endl;
        return 0;
    }

    cout << "Префикс-функция для \"" << P << "\":\n";
    vector<int> pi(m, 0);
    for (int i = 1; i < m; ++i) {
        cout << "i=" << i << " P[i]=" << P[i] << "\n";
        int j = pi[i-1];
        cout << "  j=" << j << "\n";
        while (j > 0 && P[i] != P[j]) {
            cout << "  сравнение P[i]=" << P[i] << " и P[" << j << "]=" << P[j] << " не равно, откат j=pi[" << j-1 << "]=" << pi[j-1] << "\n";
            j = pi[j-1];
        }
        if (P[i] == P[j]) {
            cout << "  сравнение P[i]=" << P[i] << " и P[" << j << "]=" << P[j] << " равно, j++ -> " << j+1 << "\n";
            j++;
        } else {
            cout << "  сравнение P[i]=" << P[i] << " и P[" << j << "]=" << P[j] << " не равно, j остаётся 0\n";
        }
        pi[i] = j;
        cout << "  pi[" << i << "]=" << j << "\n";
    }
    cout << "Готовая префикс-функция: ";
    for (int v : pi) cout << v << " ";
    cout << "\n\nПоиск вхождений:\n";

    vector<int> ans;
    int j = 0;
    for (int i = 0; i < (int)T.size(); ++i) {
        cout << "i=" << i << " T[i]=" << T[i] << " j=" << j << "\n";
        while (j > 0 && T[i] != P[j]) {
            cout << "  сравнение T[i]=" << T[i] << " и P[j]=" << P[j] << " не равно, j=pi[" << j-1 << "]=" << pi[j-1] << "\n";
            j = pi[j-1];
        }
        if (T[i] == P[j]) {
            cout << "  сравнение T[i]=" << T[i] << " и P[j]=" << P[j] << " равно, j++ -> " << j+1 << "\n";
            j++;
        } else {
            cout << "  сравнение T[i]=" << T[i] << " и P[j]=" << P[j] << " не равно, j остается " << j << "\n";
        }
        if (j == m) {
            int pos = i - m + 1;
            cout << "  *** вхождение на позиции " << pos << " ***\n";
            ans.push_back(pos);
            j = pi[j-1];
            cout << "  после совпадения j = pi[" << m-1 << "] = " << j << "\n";
        }
    }

    if (ans.empty()) {
        cout << "-1" << endl;
    } else {
        for (size_t k = 0; k < ans.size(); ++k) {
            if (k) cout << ",";
            cout << ans[k];
        }
        cout << endl;
    }
    return 0;
}