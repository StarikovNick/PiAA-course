#ifndef TSP_ALGORITHMS_H
#define TSP_ALGORITHMS_H

#include <vector>

int tsp_dp(const std::vector<std::vector<int>>& graph, std::vector<int>& path);
int tsp_alsh1(const std::vector<std::vector<int>>& graph, std::vector<int>& path);

#endif