/*
* bfs.cpp
* Author: Kyley Holder
* Description: File containing implementation of Breadth-First Search (BFS) algorithm
* Date: November 1, 2025
*/
#include "bfs.h"

std::pair<std::vector<std::string>, double> bfs(const Graph& graph, const std::string& src, const std::string& dest) {
    const std::unordered_map<std::string, std::vector<Edge>>& adj = graph.getAdjacencyList();
    if(!adj.count(src) || !adj.count(dest)) {
        return {};
    }


    if(src == dest) {
        return {{src}, 0.0};
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

    //if not found return the empty path
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

    //loop to accumulate distance
    double total_distance = 0.0;
    for(int i = 0; i + 1 < path.size(); ++i) {
        const std::string& u = path[i];
        const std::string& v = path[i + 1];

        for(const auto& edge : adj.at(u)) {
            if(edge.neighbor == v) {
                total_distance += edge.weight;
                break;
            }
        }
    }

    //returning a pair of a vector with the nodes along the path and the total distance (not in miles)
    return {path, total_distance};
}