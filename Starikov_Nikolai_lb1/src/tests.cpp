#include <gtest/gtest.h>
#include <vector>
#include <array>
using namespace std;

int solve(int N, vector<array<int, 3>> &result);

TEST(SquarePackingTest, EvenNumbers) {
    vector<array<int, 3>> result;
    EXPECT_EQ(solve(2, result), 4);
    EXPECT_EQ(solve(4, result), 4);
    EXPECT_EQ(solve(6, result), 4);
    EXPECT_EQ(solve(8, result), 4);
    EXPECT_EQ(solve(10, result), 4);
}

TEST(SquarePackingTest, MultiplesOf3) {
    vector<array<int, 3>> result;
    EXPECT_EQ(solve(3, result), 6);
    EXPECT_EQ(solve(9, result), 6);
    EXPECT_EQ(solve(15, result), 6);
    EXPECT_EQ(solve(21, result), 6);
}

TEST(SquarePackingTest, MultiplesOf5) {
    vector<array<int, 3>> result;
    EXPECT_EQ(solve(5, result), 8);
    EXPECT_EQ(solve(25, result), 8);
    EXPECT_EQ(solve(35, result), 8);
}

TEST(SquarePackingTest, KnownValues) {
    vector<array<int, 3>> result;
    EXPECT_EQ(solve(7, result), 9);
    EXPECT_EQ(solve(11, result), 11);
}

TEST(SquarePackingTest, LargePrimes) {
    vector<array<int, 3>> result;
    for (int N : {19, 29, 37}) {
        int val = solve(N, result);
        EXPECT_LE(val, N*N);
        EXPECT_GE(val, 4);
        EXPECT_NE(val, 1);
        EXPECT_NE(val, 2);
        EXPECT_NE(val, 3);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
