#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using namespace std;


int minEditCost(const string& A, const string& B, int cost_replace, int cost_insert, int cost_delete, int cost_replace1to2) {
    int n = A.size();
    int m = B.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    
    for (int i = 1; i <= n; ++i) dp[i][0] = dp[i-1][0] + cost_delete;
    for (int j = 1; j <= m; ++j) dp[0][j] = dp[0][j-1] + cost_insert;
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int del = dp[i-1][j] + cost_delete;
            int ins = dp[i][j-1] + cost_insert;
            int rep = dp[i-1][j-1] + (A[i-1] == B[j-1] ? 0 : cost_replace);
            int rep2 = (j >= 2) ? dp[i-1][j-2] + cost_replace1to2 : INT_MAX;
            dp[i][j] = min({del, ins, rep, rep2});
        }
    }
    return dp[n][m];
}

TEST(EditDistanceTest, StandardLevenshtein) {
    int r=1, i=1, d=1, r2=100;
    EXPECT_EQ(minEditCost("kitten", "sitting", r,i,d,r2), 3);
    EXPECT_EQ(minEditCost("sunday", "saturday", r,i,d,r2), 3);
    EXPECT_EQ(minEditCost("pedestal", "stien", r,i,d,r2), 7);
}

TEST(EditDistanceTest, Replace1to2Cheaper) {
    int r=1, i=1, d=1, r2=1;
    EXPECT_EQ(minEditCost("x", "yz", r,i,d,r2), 1);
    EXPECT_EQ(minEditCost("a", "bc", r,i,d,r2), 1);
}

TEST(EditDistanceTest, Replace1to2NotUsed) {
    int r=1, i=1, d=1, r2=3;
    EXPECT_EQ(minEditCost("x", "yz", r,i,d,r2), 2);
    EXPECT_EQ(minEditCost("a", "bc", r,i,d,r2), 2);
}

TEST(EditDistanceTest, EmptyStrings) {
    int r=1, i=1, d=1, r2=1;
    EXPECT_EQ(minEditCost("", "", r,i,d,r2), 0);
    EXPECT_EQ(minEditCost("abc", "", r,i,d,r2), 3);
    EXPECT_EQ(minEditCost("", "def", r,i,d,r2), 3);
}

TEST(EditDistanceTest, Identity) {
    int r=5, i=5, d=5, r2=100;
    EXPECT_EQ(minEditCost("abc", "abc", r,i,d,r2), 0);
    EXPECT_EQ(minEditCost("", "", r,i,d,r2), 0);
}

TEST(EditDistanceTest, OnlyInsertDelete) {
    int r=10, i=1, d=1, r2=100;
    EXPECT_EQ(minEditCost("ab", "abc", r,i,d,r2), 1);
    EXPECT_EQ(minEditCost("abc", "ab", r,i,d,r2), 1);
}

TEST(EditDistanceTest, Replace1to2WithLongerStrings) {
    int r=1, i=1, d=1, r2=1;
    EXPECT_EQ(minEditCost("ab", "cde", r,i,d,r2), 2);
}

TEST(EditDistanceTest, UseMultipleReplace1to2) {
    int r=1, i=1, d=1, r2=1;
    EXPECT_EQ(minEditCost("xy", "abcd", r,i,d,r2), 2);
}

TEST(EditDistanceTest, EdgeCaseJLessThan2) {
    int r=1, i=1, d=1, r2=1;
    EXPECT_EQ(minEditCost("a", "b", r,i,d,r2), 1);
    EXPECT_EQ(minEditCost("a", "", r,i,d,r2), 1);
    EXPECT_EQ(minEditCost("", "b", r,i,d,r2), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}