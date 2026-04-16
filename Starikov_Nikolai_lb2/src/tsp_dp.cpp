#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

const int INF = 1e9;

string maskToBinary(int mask, int n) {
    string s = "";
    for (int i = n - 1; i >= 0; --i) {
        s += (mask & (1 << i)) ? '1' : '0';
    }
    return s;
}

int depth = 0;

int tsp(int mask, int last, const vector<vector<int>>& graph, vector<vector<int>>& dp, vector<vector<int>>& nextMove, int n) {
    string indent(depth * 2, ' ');

    cout << indent << "tsp(маска=" << mask << " (" << maskToBinary(mask, n) << "), последний=" << last << ")\n";

    if (dp[mask][last] != -1) {
        cout << indent << "  уже вычислено: dp[" << mask << "][" << last << "] = " << dp[mask][last] << "\n";
        return dp[mask][last];
    }

    if (mask == (1 << n) - 1) {
        cout << indent << "  все города посещены\n";
        if (graph[last][0] != 0) {
            cout << indent << "    ребро " << last << " → 0 есть, стоимость = " << graph[last][0] << "\n";
            nextMove[mask][last] = 0;
            dp[mask][last] = graph[last][0];
            cout << indent << "  возврат dp[" << mask << "][" << last << "] = " << dp[mask][last] << ", nextMove = 0\n";
            return dp[mask][last];
        }
        else {
            cout << indent << "    нет ребра " << last << " → 0, возвращаем INF\n";
            dp[mask][last] = INF;
            cout << indent << "  ← возврат dp[" << mask << "][" << last << "] = INF\n";
            return INF;
        }
    }

    int best = INF;
    int bestNext = -1;
    cout << indent << "  перебираем возможные следующие города:\n";

    for (int j = 0; j < n; ++j) {
        if (mask & (1 << j)) {
            cout << indent << "    город " << j << " уже в маске, пропускаем\n";
            continue;
        }
        if (graph[last][j] == 0) {
            cout << indent << "    нет прямого пути " << last << " → " << j << ", пропускаем\n";
            continue;
        }
        int newMask = mask | (1 << j);
        cout << indent << "    пробуем j=" << j << ", ребро " << last << "→" << j << " стоимость " << graph[last][j] << "\n";
        depth++;
        int subCost = tsp(newMask, j, graph, dp, nextMove, n);
        depth--;
        int cost = graph[last][j] + subCost;
        cout << indent << "      tsp(маска=" << newMask << ", последний=" << j << ") вернул " << subCost
             << ", общая стоимость = " << graph[last][j] << " + " << subCost << " = " << cost << "\n";
        if (cost < best) {
            best = cost;
            bestNext = j;
            cout << indent << "      *** новый лучший: стоимость = " << best << ", следующий город = " << bestNext << "\n";
        }
    }

    if (bestNext == -1) {
        cout << indent << "  не найдено ни одного подходящего перехода, best остался INF\n";
    }
    else {
        cout << indent << "  лучший переход: j=" << bestNext << ", стоимость=" << best << "\n";
    }

    nextMove[mask][last] = bestNext;
    dp[mask][last] = best;
    cout << indent << "  возврат dp[" << mask << "][" << last << "] = " << best << ", nextMove = " << bestNext << "\n";
    return best;
}

vector<vector<int>> readMatrixFromFile(const string& filename) {
    ifstream input(filename);
    if (!input.is_open()) {
        cerr << "Не удалось открыть файл " << filename << "\n";
        return {};
    }
    int n;
    input >> n;
    vector<vector<int>> matrix(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            input >> matrix[i][j];
        }
    }
    input.close();
    return matrix;
}

int main(int argc, char* argv[]) {
    string filename;
    if (argc >= 2) {
        filename = argv[1];
    }
    else {
        cout << "Введите имя файла с матрицей: ";
        cin >> filename;
    }

    vector<vector<int>> graph = readMatrixFromFile(filename);
    if (graph.empty()) return 1;

    int n = graph.size();
    cout << "Матрица прочитана из файла " << filename << ":\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << graph[i][j] << " ";
        }
        cout << "\n";
    }

    if (n < 2) {
        cerr << "Количество городов должно быть не менее 2\n";
        return 1;
    }

    cout << "\n=== Начало работы точного алгоритма ===\n\n";

    vector<vector<int>> dp(1 << n, vector<int>(n, -1));
    vector<vector<int>> nextMove(1 << n, vector<int>(n, -1));

    int startMask = 1 << 0;
    int startCity = 0;
    cout << "Стартовая маска: " << startMask << " (" << maskToBinary(startMask, n) << "), стартовый город: " << startCity << "\n\n";
    int bestCost = tsp(startMask, startCity, graph, dp, nextMove, n);

    cout << "\n=== Результат ===\n";
    if (bestCost >= INF) {
        cout << "no path\n";
        return 0;
    }

    cout << "Минимальная стоимость: " << bestCost << "\n";

    cout << "\n=== Восстановление пути ===\n";
    vector<int> path;
    int mask = startMask;
    int current = startCity;
    cout << "Начинаем с mask=" << mask << " (" << maskToBinary(mask, n) << "), current=" << current << "\n";
    while (true) {
        path.push_back(current);
        cout << "  Добавляем город " << current << " в путь\n";
        if (mask == (1 << n) - 1) {
            cout << "  Маска стала полной, выходим\n";
            break;
        }
        int next = nextMove[mask][current];
        if (next == -1) {
            cout << "  Ошибка: nextMove = -1, прерывание\n";
            break;
        }
        cout << "  nextMove[" << mask << "][" << current << "] = " << next << "\n";
        mask |= (1 << next);
        current = next;
        cout << "  Новая маска: " << mask << " (" << maskToBinary(mask, n) << "), текущий город: " << current << "\n";
    }
    path.push_back(0);
    cout << "  Добавляем возврат в город 0\n";

    cout << "\nОптимальный путь: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i] << (i + 1 == path.size() ? "\n" : " ");
    }

    return 0;
}