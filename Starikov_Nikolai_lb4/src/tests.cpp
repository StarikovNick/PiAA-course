#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace std;

vector<int> findAllOccurrences(const string& P, const string& T) {
    int m = P.size();
    if (m == 0) return {};

    vector<int> pi(m, 0);
    for (int i = 1; i < m; ++i) {
        int j = pi[i - 1];
        while (j > 0 && P[i] != P[j])
            j = pi[j - 1];
        if (P[i] == P[j])
            ++j;
        pi[i] = j;
    }

    vector<int> ans;
    int j = 0;
    for (int i = 0; i < (int)T.size(); ++i) {
        while (j > 0 && T[i] != P[j])
            j = pi[j - 1];
        if (T[i] == P[j])
            ++j;
        if (j == m) {
            ans.push_back(i - m + 1);
            j = pi[j - 1];
        }
    }
    return ans;
}

int cyclicShiftIndex(const string& A, const string& B) {
    if (A.size() != B.size()) return -1;
    int n = A.size();
    if (n == 0) return 0; 

    vector<int> pi(n, 0);
    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && B[i] != B[j])
            j = pi[j - 1];
        if (B[i] == B[j])
            ++j;
        pi[i] = j;
    }

    int j = 0;
    for (int i = 0; i < 2 * n; ++i) {
        char cur = A[i % n];
        while (j > 0 && cur != B[j])
            j = pi[j - 1];
        if (cur == B[j])
            ++j;
        if (j == n) {
            int pos = i - n + 1;
            if (pos < n)
                return pos;
            j = pi[j - 1];
        }
    }
    return -1;
}

TEST(KMPTest, SimpleOccurrence) {
    string P = "ab";
    string T = "abab";
    vector<int> expected = {0, 2};
    EXPECT_EQ(findAllOccurrences(P, T), expected);
}

TEST(KMPTest, NoOccurrence) {
    string P = "abc";
    string T = "abab";
    vector<int> expected = {};
    EXPECT_EQ(findAllOccurrences(P, T), expected);
}

TEST(KMPTest, OverlappingOccurrences) {
    string P = "aaa";
    string T = "aaaaa";
    vector<int> expected = {0, 1, 2};
    EXPECT_EQ(findAllOccurrences(P, T), expected);
}

TEST(KMPTest, EmptyPattern) {
    string P = "";
    string T = "anything";
    EXPECT_EQ(findAllOccurrences(P, T), vector<int>());
}

TEST(KMPTest, PatternEqualsText) {
    string P = "abc";
    string T = "abc";
    vector<int> expected = {0};
    EXPECT_EQ(findAllOccurrences(P, T), expected);
}

TEST(KMPTest, SingleCharacterPattern) {
    string P = "a";
    string T = "banana";
    vector<int> expected = {1, 3, 5};
    EXPECT_EQ(findAllOccurrences(P, T), expected);
}

TEST(CyclicShiftTest, ExampleFromTask) {
    string A = "defabc";
    string B = "abcdef";
    EXPECT_EQ(cyclicShiftIndex(A, B), 3);
}

TEST(CyclicShiftTest, NoShiftDifferentLength) {
    string A = "abc";
    string B = "abcd";
    EXPECT_EQ(cyclicShiftIndex(A, B), -1);
}

TEST(CyclicShiftTest, EqualStrings) {
    string A = "abc";
    string B = "abc";
    EXPECT_EQ(cyclicShiftIndex(A, B), 0);
}

TEST(CyclicShiftTest, ShiftByOne) {
    string A = "cab";
    string B = "abc";
    EXPECT_EQ(cyclicShiftIndex(A, B), 1);
}

TEST(CyclicShiftTest, PeriodicStringMultipleShifts) {
    string A = "abab";
    string B = "abab";
    EXPECT_EQ(cyclicShiftIndex(A, B), 0);
}

TEST(CyclicShiftTest, ShiftOnPeriodicString) {
    string A = "baba";
    string B = "abab";
    EXPECT_EQ(cyclicShiftIndex(A, B), 1);
}

TEST(CyclicShiftTest, EmptyStrings) {
    string A = "";
    string B = "";
    EXPECT_EQ(cyclicShiftIndex(A, B), 0);
}

TEST(CyclicShiftTest, OneEmpty) {
    string A = "a";
    string B = "";
    EXPECT_EQ(cyclicShiftIndex(A, B), -1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}