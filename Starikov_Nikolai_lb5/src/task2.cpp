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

    string T, P, wc_str;
    cin >> T >> P >> wc_str;
    char wildcard = wc_str[0];

    cout << "=== Входные данные ===\n";
    cout << "Текст T: " << T << "\n";
    cout << "Шаблон P: " << P << "\n";
    cout << "Символ джокера: " << wildcard << "\n\n";

    int m = P.size();
    vector<string> segs;         
    vector<int> offsets;
    for (int i = 0; i < m; ++i) {
        if (P[i] != wildcard) {
            int j = i;
            while (j < m && P[j] != wildcard) ++j;
            segs.push_back(P.substr(i, j - i));
            offsets.push_back(i);
            i = j - 1;
        }
    }
    int seg_cnt = segs.size();
    cout << "Сегменты (без джокеров):\n";
    for (int id = 0; id < seg_cnt; ++id) {
        cout << "  " << id+1 << ": \"" << segs[id] << "\" (смещение " << offsets[id] << ")\n";
    }
    cout << "\n";

    vector<Node> trie(1);
    cout << "=== Построение бора ===\n";
    for (int id = 0; id < seg_cnt; ++id) {
        int cur = 0;
        cout << "Добавление сегмента " << id+1 << " (\"" << segs[id] << "\"):\n";
        for (char ch : segs[id]) {
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
        trie[cur].pats.push_back(id);  
        cout << "  Отмечен конец сегмента " << id+1 << " в вершине " << cur << "\n";
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
        cout << "  Сегменты в вершине: ";
        if (trie[v].pats.empty()) {
            cout << "(нет)";
        } else {
            for (size_t i = 0; i < trie[v].pats.size(); ++i) {
                cout << trie[v].pats[i] + 1 << (i+1 < trie[v].pats.size() ? " " : "");
            }
        }
        cout << "\n\n";
    }

    cout << "=== Процесс поиска сегментов ===\n";
    vector<pair<int, int>> cand_matches;

    int state = 0;
    int text_len = T.size();

    for (int pos = 0; pos < text_len; ++pos) {
        char ch = T[pos];
        int c = charToIdx(ch);
        state = trie[state].next[c];

        cout << "Позиция " << pos+1 << ", символ '" << ch << "', переход в состояние " << state << "\n";

        vector<int> found;
        if (!trie[state].pats.empty()) {
            for (int id : trie[state].pats) found.push_back(id);
        }
        int out = trie[state].output;
        while (out != 0) {
            for (int id : trie[out].pats) found.push_back(id);
            out = trie[out].output;
        }

        if (!found.empty()) {
            cout << "  Найдены сегменты: ";
            for (int id : found) {
                int seg_len = segs[id].size();
                int start_in_pattern = offsets[id];
                int cand_start = pos - seg_len + 1 - start_in_pattern;
                if (cand_start >= 0 && cand_start + m <= text_len) {
                    cand_matches.emplace_back(cand_start, id);
                    cout << "сегмент " << id+1 << " (возможное начало шаблона " << cand_start+1 << ") ";
                }
            }
            cout << "\n";
        }
    }
    cout << "Поиск сегментов завершён.\n\n";

    sort(cand_matches.begin(), cand_matches.end());

    vector<int> results;
    int i = 0;
    while (i < (int)cand_matches.size()) {
        int start = cand_matches[i].first;
        vector<bool> seen(seg_cnt, false);
        int cnt = 0;
        while (i < (int)cand_matches.size() && cand_matches[i].first == start) {
            int id = cand_matches[i].second;
            if (!seen[id]) {
                seen[id] = true;
                ++cnt;
            }
            ++i;
        }
        if (cnt == seg_cnt) {
            results.push_back(start + 1);
        }
    }

    cout << "=== РЕЗУЛЬТАТЫ ВАРИАНТА 2 ===\n";
    cout << "Количество вершин в автомате: " << trie.size() << "\n";

    struct Occ {
        int start, end, id;
    };
    vector<Occ> occs;
    for (const auto& cm : cand_matches) {
        int start = cm.first;
        int id = cm.second;
        int seg_len = segs[id].size();
        int end = start + seg_len - 1;
        occs.push_back({start, end, id});
    }
    sort(occs.begin(), occs.end(), [](const Occ& a, const Occ& b) { return a.start < b.start; });

    vector<bool> hasIntersection(seg_cnt, false);
    for (size_t i = 0; i < occs.size(); ++i) {
        for (size_t j = i+1; j < occs.size(); ++j) {
            if (occs[j].start > occs[i].end) break;
            if (occs[i].id != occs[j].id) {
                hasIntersection[occs[i].id] = true;
                hasIntersection[occs[j].id] = true;
            }
        }
    }

    vector<int> intersectingSegments;
    for (int id = 0; id < seg_cnt; ++id) {
        if (hasIntersection[id]) intersectingSegments.push_back(id+1);
    }

    if (intersectingSegments.empty()) {
        cout << "Список сегментов, имеющих пересечения с другими: (нет)\n";
    } else {
        cout << "Список сегментов, имеющих пересечения с другими: ";
        for (size_t i = 0; i < intersectingSegments.size(); ++i) {
            cout << intersectingSegments[i] << (i+1 < intersectingSegments.size() ? " " : "");
        }
        cout << "\n";
    }
    cout << "\n";

    cout << "=== Вхождение шаблона (стандартный вывод) ===\n";
    for (int pos : results) {
        cout << pos << "\n";
    }

    return 0;
}