#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

using namespace std;

int table_size;
vector<unsigned long long> board_mask;
int filled_cells;
int best_solution_count;
vector<array<int, 3>> best_solution;
vector<array<int, 3>> current_solution;

int recursion_depth = 0;
int operation_counter = 0;

bool canPlace(int row, int col, int square_size) {
    if (row + square_size > table_size || col + square_size > table_size) return false;
    unsigned long long mask = ((1ULL << square_size) - 1) << col;
    for (int i = row; i < row + square_size; ++i) {
        if (board_mask[i] & mask) return false;
    }
    return true;
}

void placeSquare(int row, int col, int square_size, bool place) {
    unsigned long long mask = ((1ULL << square_size) - 1) << col;
    for (int i = row; i < row + square_size; ++i) {
        if (place) board_mask[i] |= mask;
        else board_mask[i] &= ~mask;
    }
    filled_cells += place ? square_size * square_size : -square_size * square_size;
}

bool findFirstFree(int &row, int &col) {
    for (row = 0; row < table_size; ++row) {
        unsigned long long free = ~board_mask[row] & ((1ULL << table_size) - 1);
        if (free) {
            col = __builtin_ctzll(free);
            return true;
        }
    }
    return false;
}

void backtrack(int used_squares) {
    string indent(recursion_depth * 2, ' ');
    cout << indent << "backtrack(used=" << used_squares << ")\n";
    operation_counter++;

    if (used_squares >= best_solution_count) {
        cout << indent << " отсечение: used >= best\n";
        return;
    }

    int row, col;
    if (!findFirstFree(row, col)) {
        if (used_squares < best_solution_count) {
            cout << indent << "НОВОЕ ЛУЧШЕЕ РЕШЕНИЕ! Квадратов = " << used_squares << "\n";
            best_solution_count = used_squares;
            best_solution = current_solution;
        }
        return;
    }

    int remaining_cells = table_size * table_size - filled_cells;
    int max_possible_size = min({table_size - row, table_size - col, table_size - 1});
    int min_additional = (remaining_cells + max_possible_size * max_possible_size - 1) / (max_possible_size * max_possible_size);

    if (used_squares + min_additional >= best_solution_count) {
        cout << indent << " отсечение по нижней границе (min_additional=" << min_additional << ")\n";
        return;
    }

    for (int size = max_possible_size; size >= 1; --size) {
        if (!canPlace(row, col, size)) continue;

        cout << indent << " ставим квадрат " << size << " в (" << row << "," << col << ")";
        placeSquare(row, col, size, true);
        current_solution.push_back({row + 1, col + 1, size});
        cout << " (занято " << filled_cells << "/" << table_size*table_size << ")\n";

        recursion_depth++;
        backtrack(used_squares + 1);
        recursion_depth--;

        cout << indent << " убираем квадрат " << size << " из (" << row << "," << col << ")\n";
        current_solution.pop_back();
        placeSquare(row, col, size, false);
    }
}

int main() {
    cin >> table_size;

    if (table_size % 2 == 0) {
        int half = table_size / 2;
        cout << "4\n";
        cout << "1 1 " << half << "\n1 " << half+1 << " " << half << "\n";
        cout << half+1 << " 1 " << half << "\n" << half+1 << " " << half+1 << " " << half << "\n";
        return 0;
    }

    if (table_size % 3 == 0) {
        int third = table_size / 3;
        int two = 2 * third;
        cout << "6\n";
        cout << "1 1 " << two << "\n";
        cout << "1 " << two+1 << " " << third << "\n";
        cout << third+1 << " " << two+1 << " " << third << "\n";
        cout << two+1 << " 1 " << third << "\n";
        cout << two+1 << " " << third+1 << " " << third << "\n";
        cout << two+1 << " " << two+1 << " " << third << "\n";
        return 0;
    }

    if (table_size % 5 == 0) {
        int fifth = table_size / 5;
        int three = 3 * fifth;
        int two = 2 * fifth;
        cout << "8\n";
        cout << "1 1 " << three << "\n";
        cout << "1 " << three+1 << " " << two << "\n";
        cout << two+1 << " " << three+1 << " " << fifth << "\n";
        cout << two+1 << " " << three+fifth+1 << " " << fifth << "\n";
        cout << three+1 << " 1 " << two << "\n";
        cout << three+1 << " " << two+1 << " " << two << "\n";
        cout << three+1 << " " << 2*two+1 << " " << fifth << "\n";
        cout << three+fifth+1 << " " << 2*two+1 << " " << fifth << "\n";
        return 0;
    }

    cout << "=== Запуск для N = " << table_size << " ===\n";
    board_mask.assign(table_size, 0);
    filled_cells = 0;
    best_solution_count = table_size * table_size;
    current_solution.reserve(table_size * table_size);
    best_solution.reserve(table_size * table_size);

    int big = table_size / 2 + 1;
    int small = table_size / 2;
    cout << "Эвристика: ставим квадраты " << big << ", " << small << ", " << small << "\n";
    placeSquare(0, 0, big, true);
    current_solution.push_back({1, 1, big});
    placeSquare(0, big, small, true);
    current_solution.push_back({1, big+1, small});
    placeSquare(big, 0, small, true);
    current_solution.push_back({big+1, 1, small});

    backtrack(3);

    cout << "\n=== Лучшее решение ===";
    cout << "\nКоличество квадратов: " << best_solution_count << "\n";
    for (auto &sq : best_solution)
        cout << sq[0] << " " << sq[1] << " " << sq[2] << "\n";


    return 0;
}
