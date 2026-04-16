#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <climits>

using namespace std;

string visitedToString(const vector<bool>& visited) {
    string s = "";
    for (bool v : visited) s += v ? '1' : '0';
    return s;
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
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            input >> matrix[i][j];
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
    cout << "\nМатрица расстояний, прочитанная из файла " << filename << ":\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            cout << graph[i][j] << " ";
        cout << "\n";
    }

    if (n < 2) {
        cerr << "Количество городов должно быть не менее 2\n";
        return 1;
    }

    cout << "\n=== Начало работы алгоритма АЛШ-1 (жадный ближайший сосед) ===\n\n";

    vector<bool> visited(n, false);
    vector<int> path;
    int current = 0;
    path.push_back(current);
    visited[current] = true;
    int totalCost = 0;
    bool possible = true;

    cout << "Стартовый город: " << current << "\n";
    cout << "Посещённые: " << visitedToString(visited) << "\n\n";

    for (int step = 1; step < n; ++step) {
        int nearest = -1;
        int minDist = INT_MAX;

        cout << "Шаг " << step << ": находимся в городе " << current << "\n";
        cout << "  Ищем ближайший непосещённый город:\n";

        for (int j = 0; j < n; ++j) {
            if (!visited[j]) {
                if (graph[current][j] == 0) {
                    cout << "    Город " << j << " — нет прямого пути, пропускаем\n";
                }
                else {
                    cout << "    Город " << j << " — расстояние " << graph[current][j];
                    if (graph[current][j] < minDist) {
                        cout << " (новый минимум)";
                        minDist = graph[current][j];
                        nearest = j;
                    }
                    cout << "\n";
                }
            }
            else {
                cout << "    Город " << j << " уже посещён, пропускаем\n";
            }
        }

        if (nearest == -1) {
            cout << "  Не найден ни один доступный город! Маршрут невозможен.\n";
            possible = false;
            break;
        }

        cout << "  Выбран ближайший город: " << nearest << " с расстоянием " << minDist << "\n";
        totalCost += minDist;
        current = nearest;
        path.push_back(current);
        visited[current] = true;
        cout << "  Переходим в город " << current << ", текущая стоимость маршрута: " << totalCost << "\n";
        cout << "  Посещённые: " << visitedToString(visited) << "\n\n";
    }

    if (possible) {
        cout << "Все города посещены. Пытаемся вернуться в город 0...\n";
        if (graph[current][0] != 0) {
            totalCost += graph[current][0];
            path.push_back(0);
            cout << "  Ребро " << current << " -> 0 существует, стоимость = " << graph[current][0] << "\n";
            cout << "  Итоговая стоимость маршрута: " << totalCost << "\n";
        }
        else {
            cout << "  Нет ребра из " << current << " в 0! Замкнуть цикл невозможно.\n";
            possible = false;
        }
    }

    cout << "\n=== Результат работы АЛШ-1 ===\n";
    if (!possible) {
        cout << "ALSH-1: no path\n";
    }
    else {
        cout << "ALSH-1 cost: " << totalCost << "\n";
        cout << "Маршрут: ";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i] << (i + 1 == path.size() ? "\n" : " ");
        }
    }

    return 0;
}