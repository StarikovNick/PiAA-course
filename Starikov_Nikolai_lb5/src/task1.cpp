#include <iostream>
#include <vector>
#include <queue>
#include <array>
#include <algorithm>
#include <string>

using namespace std;

const int ALPH = 5;
const char ALPH_SYMBOLS[] = {'A', 'C', 'G', 'T', 'N'};

int charToIdx(char c) {
    switch (c) {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        case 'T': return 3;
        case 'N': return 4;
        default: return -1;
    }
}

struct Node {
    array<int, ALPH> next;
    int fail;
    int output;
    vector<int> pats;

    Node() {
        next.fill(-1);
        fail = 0;
        output = 0;
    }
};

int main() {

    string T;
    cin >> T;
    int n;
    cin >> n;
    vector<string> patterns(n);
    for (int i = 0; i < n; ++i) {
        cin >> patterns[i];
    }

    cout << "=== Входные данные ===\n";
    cout << "Текст T: " << T << "\n";
    cout << "Количество образцов: " << n << "\n";
    for (int i = 0; i < n; ++i) {
        cout << "  образец " << i+1 << ": " << patterns[i] << "\n";
    }
    cout << "\n";

    vector<Node> trie(1);
    cout << "=== Построение бора ===\n";

    for (int i = 0; i < n; ++i) {
        int cur = 0;
        cout << "Добавление образца " << i+1 << " (\"" << patterns[i] << "\"):\n";
        for (char ch : patterns[i]) {
            int c = charToIdx(ch);
            if (trie[cur].next[c] == -1) {
                trie[cur].next[c] = trie.size();
                trie.emplace_back();
                cout << "  Символ '" << ch << "' -> новая вершина " << trie.size()-1 << "\n";
            } else {
                cout << "  Символ '" << ch << "' -> существующая вершина " << trie[cur].next[c] << "\n";
            }
            cur = trie[cur].next[c];
        }
        trie[cur].pats.push_back(i+1);
        cout << "  Отмечен конец образца " << i+1 << " в вершине " << cur << "\n";
    }
    cout << "Построение бора завершено. Количество вершин: " << trie.size() << "\n\n";

    cout << "=== Построение автомата ===\n";
    queue<int> q;

    for (int c = 0; c < ALPH; ++c) {
        if (trie[0].next[c] != -1) {
            int child = trie[0].next[c];
            trie[child].fail = 0;
            trie[child].output = 0;
            q.push(child);
            cout << "Вершина " << child << ": fail = 0, output = 0 (прямой потомок корня)\n";
        } else {
            trie[0].next[c] = 0;
        }
    }

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int c = 0; c < ALPH; ++c) {
            int u = trie[v].next[c];
            if (u != -1) {
                int f = trie[v].fail;
                int ff = trie[f].next[c];
                trie[u].fail = ff;

                if (!trie[ff].pats.empty()) {
                    trie[u].output = ff;
                } else {
                    trie[u].output = trie[ff].output;
                }

                cout << "Вершина " << u << ": символ '" << ALPH_SYMBOLS[c] << "', fail = " << ff
                     << ", output = " << trie[u].output << "\n";
                q.push(u);
            } else {
                trie[v].next[c] = trie[trie[v].fail].next[c];
            }
        }
    }
    cout << "Построение автомата завершено.\n\n";

    cout << "=== Построенный автомат (все вершины) ===\n";
    for (size_t v = 0; v < trie.size(); ++v) {
        cout << "Вершина " << v << ":\n";
        cout << "  Переходы: ";
        for (int c = 0; c < ALPH; ++c) {
            cout << ALPH_SYMBOLS[c] << "->" << trie[v].next[c];
            if (c < ALPH-1) cout << ", ";
        }
        cout << "\n";
        cout << "  Суффиксная ссылка (fail): " << trie[v].fail << "\n";
        cout << "  Выходная ссылка (output): " << trie[v].output << "\n";
        cout << "  Образцы в вершине: ";
        if (trie[v].pats.empty()) {
            cout << "(нет)";
        } else {
            for (size_t i = 0; i < trie[v].pats.size(); ++i) {
                cout << trie[v].pats[i] << (i+1 < trie[v].pats.size() ? " " : "");
            }
        }
        cout << "\n\n";
    }

    cout << "=== Процесс поиска ===\n";
    vector<pair<int, int>> matches;

    int state = 0;
    for (int pos = 0; pos < (int)T.size(); ++pos) {
        char ch = T[pos];
        int c = charToIdx(ch);
        state = trie[state].next[c];

        cout << "Позиция " << pos+1 << ", символ '" << ch << "', переход в состояние " << state << "\n";

        vector<int> found;

        if (!trie[state].pats.empty()) {
            for (int pid : trie[state].pats) {
                found.push_back(pid);
            }
        }
        int out = trie[state].output;
        while (out != 0) {
            for (int pid : trie[out].pats) {
                found.push_back(pid);
            }
            out = trie[out].output;
        }

        if (!found.empty()) {
            cout << "  Найдены образцы: ";
            for (int pid : found) {
                int len = patterns[pid-1].size();
                int start = pos - len + 2;
                matches.emplace_back(start, pid);
                cout << pid << " (позиция " << start << ") ";
            }
            cout << "\n";
        }
    }
    cout << "Поиск завершён.\n\n";

    cout << "=== РЕЗУЛЬТАТЫ ВАРИАНТА 2 ===\n";

    cout << "Количество вершин в автомате: " << trie.size() << "\n";

    struct Occurrence {
        int start;
        int end;
        int pid;
    };
    vector<Occurrence> occs;
    for (const auto& m : matches) {
        int start = m.first;
        int pid = m.second;
        int len = patterns[pid-1].size();
        int end = start + len - 1;
        occs.push_back({start, end, pid});
    }
    sort(occs.begin(), occs.end(), [](const Occurrence& a, const Occurrence& b) {
        return a.start < b.start;
    });

    vector<bool> hasIntersection(n+1, false);

    for (size_t i = 0; i < occs.size(); ++i) {
        for (size_t j = i+1; j < occs.size(); ++j) {
            if (occs[j].start > occs[i].end) break;
            if (occs[i].pid != occs[j].pid) {
                hasIntersection[occs[i].pid] = true;
                hasIntersection[occs[j].pid] = true;
            }
        }
    }

    vector<int> intersectingPatterns;
    for (int pid = 1; pid <= n; ++pid) {
        if (hasIntersection[pid]) {
            intersectingPatterns.push_back(pid);
        }
    }

    if (intersectingPatterns.empty()) {
        cout << "Список образцов, имеющих пересечения с другими: (нет)\n";
    } else {
        cout << "Список образцов, имеющих пересечения с другими: ";
        for (size_t i = 0; i < intersectingPatterns.size(); ++i) {
            cout << intersectingPatterns[i] << (i+1 < intersectingPatterns.size() ? " " : "");
        }
        cout << "\n";
    }
    cout << "\n";

    cout << "=== СТАНДАРТНЫЙ ВЫВОД (позиция номер_образца) ===\n";
    sort(matches.begin(), matches.end());
    for (const auto& m : matches) {
        cout << m.first << " " << m.second << "\n";
    }

    return 0;
}
