#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <set>

using namespace std;

int table_size;
vector<unsigned long long> board_mask;
int filled_cells;
int best_solution_count;
vector<array<int, 3>> best_solution;
vector<array<int, 3>> current_solution;

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
    if (used_squares >= best_solution_count) return;
    
    int row, col;
    if (!findFirstFree(row, col)) {
        if (used_squares < best_solution_count) {
            best_solution_count = used_squares;
            best_solution = current_solution;
        }
        return;
    }
    
    int remaining_cells = table_size * table_size - filled_cells;
    int max_possible_size = min({table_size - row, table_size - col, table_size - 1});
    int min_additional_squares = (remaining_cells + max_possible_size * max_possible_size - 1) / (max_possible_size * max_possible_size);
    
    if (used_squares + min_additional_squares >= best_solution_count) return;
    
    for (int square_size = max_possible_size; square_size >= 1; --square_size) {
        if (!canPlace(row, col, square_size)) continue;
    
        placeSquare(row, col, square_size, true);
        current_solution.push_back({row + 1, col + 1, square_size});
        backtrack(used_squares + 1);
        current_solution.pop_back();
        placeSquare(row, col, square_size, false);
}
}

int main() {
    cin >> table_size;
    
    if (table_size % 2 == 0) {
        int half = table_size / 2;
        cout << 4 << "\n";
        cout << "1 1 " << half << "\n";
        cout << "1 " << (half + 1) << " " << half << "\n";
        cout << (half + 1) << " 1 " << half << "\n";
        cout << (half + 1) << " " << (half + 1) << " " << half << "\n";
        return 0;
    }
    
    if (table_size % 3 == 0) {
        int third = table_size / 3;
        int two_thirds = 2 * third;
        cout << 6 << "\n";
        cout << "1 1 " << two_thirds << "\n";
        cout << "1 " << (two_thirds + 1) << " " << third << "\n";
        cout << (third + 1) << " " << (two_thirds + 1) << " " << third << "\n";
        cout << (two_thirds + 1) << " 1 " << third << "\n";
        cout << (two_thirds + 1) << " " << (third + 1) << " " << third << "\n";
        cout << (two_thirds + 1) << " " << (two_thirds + 1) << " " << third << "\n";
        return 0;
    }
    
    if (table_size % 5 == 0) {
        int fifth = table_size / 5;
        int three_fifths = 3 * fifth;
        int two_fifths = 2 * fifth;
        cout << 8 << "\n";
        cout << "1 1 " << three_fifths << "\n";
        cout << "1 " << (three_fifths + 1) << " " << two_fifths << "\n";
        cout << (two_fifths + 1) << " " << (three_fifths + 1) << " " << fifth << "\n";
        cout << (two_fifths + 1) << " " << (three_fifths + fifth + 1) << " " << fifth << "\n";
        cout << (three_fifths + 1) << " 1 " << two_fifths << "\n";
        cout << (three_fifths + 1) << " " << (two_fifths + 1) << " " << two_fifths << "\n";
        cout << (three_fifths + 1) << " " << (2 * two_fifths + 1) << " " << fifth << "\n";
        cout << (three_fifths + fifth + 1) << " " << (2 * two_fifths + 1) << " " << fifth << "\n";
        return 0;
    }
    
    board_mask.assign(table_size, 0);
    filled_cells = 0;
    best_solution_count = table_size * table_size;
    current_solution.reserve(table_size * table_size);
    best_solution.reserve(table_size * table_size);
    
    int big_square = table_size / 2 + 1;
    int small_square = table_size / 2;
    
    placeSquare(0, 0, big_square, true);
    current_solution.push_back({1, 1, big_square});
    
    placeSquare(0, big_square, small_square, true);
    current_solution.push_back({1, big_square + 1, small_square});
    
    placeSquare(big_square, 0, small_square, true);
    current_solution.push_back({big_square + 1, 1, small_square});
    
    backtrack(3);
    
    cout << best_solution_count << "\n";
    for (const auto &square : best_solution) {
        cout << square[0] << " " << square[1] << " " << square[2] << "\n";
    }
    
    return 0;
}
