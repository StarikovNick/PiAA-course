#include <gtest/gtest.h>
#include <vector>
#include <climits>
#include "tsp_algorithms.h"

using namespace std;

const int INF = 1e9;

bool isValidPath(const vector<int>& path, int n) {
    if (path.empty() || path.front() != 0 || path.back() != 0) return false;
    vector<bool> visited(n, false);
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int city = path[i];
        if (city < 0 || city >= n) return false;
        if (visited[city]) return false;
        visited[city] = true;
    }
    for (int i = 0; i < n; ++i) if (!visited[i]) return false;
    return true;
}

TEST(TSP_DP, SampleInput1) {
    vector<vector<int>> graph = {
        {0, 1, 13, 23, 7},
        {12, 0, 15, 18, 28},
        {21, 29, 0, 33, 28},
        {23, 19, 34, 0, 38},
        {5, 40, 7, 39, 0}
    };
    vector<int> path;
    int cost = tsp_dp(graph, path);
    EXPECT_EQ(cost, 78);
    vector<int> expected = {0, 4, 2, 3, 1, 0};
    EXPECT_EQ(path.size(), expected.size());
    for (size_t i = 0; i < path.size(); ++i)
        EXPECT_EQ(path[i], expected[i]);
}

TEST(TSP_DP, NoPath) {
    vector<vector<int>> graph = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}
    };
    vector<int> path;
    int cost = tsp_dp(graph, path);
    EXPECT_EQ(cost, INF);
}

TEST(TSP_DP, Triangle) {
    vector<vector<int>> graph = {
        {0, 2, 9},
        {2, 0, 6},
        {9, 6, 0}
    };
    vector<int> path;
    int cost = tsp_dp(graph, path);
    EXPECT_EQ(cost, 17);
    EXPECT_TRUE(isValidPath(path, 3));
    int sum = 0;
    for (size_t i = 0; i < path.size() - 1; ++i)
        sum += graph[path[i]][path[i+1]];
    EXPECT_EQ(sum, cost);
}

TEST(TSP_DP, FourCitiesComplete) {
    vector<vector<int>> graph = {
        {0, 1, 2, 3},
        {1, 0, 4, 5},
        {2, 4, 0, 6},
        {3, 5, 6, 0}
    };
    vector<int> path;
    int cost = tsp_dp(graph, path);
    EXPECT_EQ(cost, 14);
    EXPECT_TRUE(isValidPath(path, 4));
}

TEST(TSP_ALSH1, SampleInput1) {
    vector<vector<int>> graph = {
        {0, 1, 13, 23, 7},
        {12, 0, 15, 18, 28},
        {21, 29, 0, 33, 28},
        {23, 19, 34, 0, 38},
        {5, 40, 7, 39, 0}
    };
    vector<int> path;
    int cost = tsp_alsh1(graph, path);
    EXPECT_EQ(cost, 106);
    EXPECT_TRUE(isValidPath(path, 5));
    int sum = 0;
    for (size_t i = 0; i < path.size() - 1; ++i)
        sum += graph[path[i]][path[i+1]];
    EXPECT_EQ(sum, cost);
}

TEST(TSP_ALSH1, NoPath) {
    vector<vector<int>> graph = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}
    };
    vector<int> path;
    int cost = tsp_alsh1(graph, path);
    EXPECT_GE(cost, INF);
}

TEST(TSP_ALSH1, Triangle) {
    vector<vector<int>> graph = {
        {0, 2, 9},
        {2, 0, 6},
        {9, 6, 0}
    };
    vector<int> path;
    int cost = tsp_alsh1(graph, path);
    EXPECT_EQ(cost, 17);
    EXPECT_TRUE(isValidPath(path, 3));
}

TEST(TSP_ALSH1, NonOptimalCase) {
    vector<vector<int>> graph = {
        {0, 1, 2, 100},
        {1, 0, 100, 100},
        {2, 100, 0, 1},
        {100, 100, 1, 0}
    };
    vector<int> path;
    int cost = tsp_alsh1(graph, path);
    EXPECT_EQ(cost, 202);
    EXPECT_TRUE(isValidPath(path, 4));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}