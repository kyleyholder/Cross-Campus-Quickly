/*
* bfs.h
* Author: Kyley Holder
* Description: Header file for bfs.cpp
* Date: November 1, 2025
*/
#include "Graph.h"
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>

std::pair<std::vector<std::string>, double> bfs(const Graph& graph, const std::string& src, const std::string& dest);