#include "tsp_algorithms.h"
#include <vector>
#include <algorithm>
#include <climits>
#include <iostream>

using namespace std;

const int INF = 1e9;

static int tsp_dp_rec(int mask, int last, const vector<vector<int>>& graph, vector<vector<int>>& dp, vector<vector<int>>& nextMove, int n) {
    if (dp[mask][last] != -1) return dp[mask][last];
    if (mask == (1 << n) - 1) {
        if (graph[last][0] != 0) {
            nextMove[mask][last] = 0;
            return dp[mask][last] = graph[last][0];
        }
        else {
            return dp[mask][last] = INF;
        }
    }
    int best = INF;
    int bestNext = -1;
    for (int j = 0; j < n; ++j) {
        if (mask & (1 << j)) continue;
        if (graph[last][j] == 0) continue;
        int newMask = mask | (1 << j);
        int cost = graph[last][j] + tsp_dp_rec(newMask, j, graph, dp, nextMove, n);
        if (cost < best) {
            best = cost;
            bestNext = j;
        }
    }
    nextMove[mask][last] = bestNext;
    return dp[mask][last] = best;
}

int tsp_dp(const vector<vector<int>>& graph, vector<int>& path) {
    int n = graph.size();
    if (n == 0) return INF;
    vector<vector<int>> dp(1 << n, vector<int>(n, -1));
    vector<vector<int>> nextMove(1 << n, vector<int>(n, -1));

    int startMask = 1 << 0;
    int startCity = 0;
    int bestCost = tsp_dp_rec(startMask, startCity, graph, dp, nextMove, n);

    if (bestCost >= INF) {
        path.clear();
        return INF;
    }

    path.clear();
    int mask = startMask;
    int cur = startCity;
    while (true) {
        path.push_back(cur);
        if (mask == (1 << n) - 1) break;
        int nxt = nextMove[mask][cur];
        if (nxt == -1) break;
        mask |= (1 << nxt);
        cur = nxt;
    }
    path.push_back(0);
    return bestCost;
}

int tsp_alsh1(const vector<vector<int>>& graph, vector<int>& path) {
    int n = graph.size();
    if (n == 0) return INF;

    vector<bool> visited(n, false);
    path.clear();
    int current = 0;
    path.push_back(current);
    visited[current] = true;
    int totalCost = 0;
    bool possible = true;

    for (int step = 1; step < n; ++step) {
        int nearest = -1;
        int minDist = INT_MAX;
        for (int j = 0; j < n; ++j) {
            if (!visited[j] && graph[current][j] != 0 && graph[current][j] < minDist) {
                minDist = graph[current][j];
                nearest = j;
            }
        }
        if (nearest == -1) {
            possible = false;
            break;
        }
        totalCost += minDist;
        current = nearest;
        path.push_back(current);
        visited[current] = true;
    }

    if (possible && graph[current][0] != 0) {
        totalCost += graph[current][0];
        path.push_back(0);
        return totalCost;
    } else {
        path.clear();
        return INF;
    }
}