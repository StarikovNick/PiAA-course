#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

vector<vector<int>> generateRandomMatrix(int n, int minDist = 1, int maxDist = 100) {
    vector<vector<int>> mat(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) mat[i][j] = 0;
            else mat[i][j] = minDist + rand() % (maxDist - minDist + 1);
        }
    }
    return mat;
}

vector<vector<int>> generateSymmetricMatrix(int n, int minDist = 1, int maxDist = 100) {
    vector<vector<int>> mat(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int d = minDist + rand() % (maxDist - minDist + 1);
            mat[i][j] = d;
            mat[j][i] = d;
        }
    }
    return mat;
}

void saveMatrixToFile(const vector<vector<int>>& mat, const string& filename) {
    ofstream out(filename);
    int n = mat.size();
    out << n << "\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            out << mat[i][j] << (j + 1 == n ? "\n" : " ");
        }
    }
    out.close();
    cout << "Матрица сохранена в " << filename << "\n";
}

int main() {
    srand(time(0));

    cout << "=== Генератор матрицы расстояний для задачи коммивояжёра ===\n";
    int choice;
    cout << "Выберите тип матрицы:\n";
    cout << "1. Произвольная (несимметричная)\n";
    cout << "2. Симметричная\n";
    cout << "Ваш выбор: ";
    cin >> choice;

    int n;
    cout << "Введите количество городов: ";
    cin >> n;

    vector<vector<int>> mat;
    if (choice == 1) {
        mat = generateRandomMatrix(n);
        cout << "Сгенерирована произвольная матрица:\n";
    }
    else if (choice == 2) {
        mat = generateSymmetricMatrix(n);
        cout << "Сгенерирована симметричная матрица:\n";
    }
    else {
        cerr << "Неверный выбор\n";
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << mat[i][j] << " ";
        }
        cout << "\n";
    }

    string filename;
    cout << "Введите имя файла для сохранения матрицы: ";
    cin >> filename;
    saveMatrixToFile(mat, filename);

    return 0;
}