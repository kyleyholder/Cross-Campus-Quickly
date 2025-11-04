/*
* bfs.cpp
* Author: Kyley Holder
* Description: File containing implementation of Breadth-First Search (BFS) algorithm
* Date: November 1, 2025
*/
#include "bfs.h"


std::vector<std::string> bfs(const Graph& graph, const std::string& src, const std::string& dest) {
    const std::unordered_map<std::string, std::vector<Edge>>& adj = graph.getAdjacencyList();
    if(!adj.count(src) || !adj.count(dest)) {
        return {};
    }

    if(src == dest) {
        return {src};
    }

    std::queue<std::string> q;
    std::unordered_set<std::string> visited;
    std::unordered_map<std::string, std::string> parent;

    visited.insert(src);
    q.push(src);

    bool found = false;
    while(!q.empty()) {
        std::string u = q.front();
        q.pop();
        for(const auto& edge : adj.at(u)) {
            const std::string& v = edge.neighbor;
            if(!visited.count(v)) {
                visited.insert(v);
                parent[v] = u;
                if(v == dest) {
                    found = true;
                    break;
                }
                q.push(v);
            }
        }
        if(found) {
            break;
        }
    }

    if(!found) {
        return {};
    }

    std::vector<std::string> path;
    std::string cur = dest;
    while(true) {
        path.push_back(cur);
        if(cur == src) {
            break;
        }
        cur = parent[cur];
    }

    std::reverse(path.begin(), path.end());
    return path;
}