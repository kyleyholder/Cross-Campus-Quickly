/*
* Dijkstra.cpp
* Author: Shelby Coonce
* Description: File containing Dijkstra algorithm for finding the shortest paths between nodes in a weighted graph
* Date: November 3, 2025
*/

#include "Dijkstra.h"

std::pair<std::vector<std::string>, double> Dijkstra(const Graph& graph, const std::string& src, const std::string& dest) {
    const std::unordered_map<std::string, std::vector<Edge>>& adj = graph.getAdjacencyList();

    if(!adj.count(src) || !adj.count(dest)) {
        return {{}, 0.0};
    }

    if(src == dest) {
        return {{src}, 0.0};
    }

    // Visited Set
    std::unordered_set<std::string> visited;
    // Unvisted Priority Queue
    std::priority_queue<std::pair<double, std::string>, std::vector<std::pair<double, std::string>>, std::greater<std::pair<double, std::string>>> unvisited;
    unvisited.push({0, src});

    // Shorted Distance from src to dest
    std::unordered_map<std::string, double> d;
    // Predecessor of dest
    std::unordered_map<std::string, std::string> p;
    for(auto& [node, edge] : adj) {
        d[node] = std::numeric_limits<double>::infinity();
        p[node] = "";
    }
    d[src] = 0;

    while(!unvisited.empty()) {
        // Find Node with Smallest Distance
        auto [dist, u] = unvisited.top();
        unvisited.pop();

        if(u == dest)
            break;

        if (visited.count(u))
            continue;
        visited.insert(u);

        // Relax Neighbors
        for(const auto& edge : adj.at(u)) {
            if(d[edge.neighbor] > d[u] + edge.weight) {
                d[edge.neighbor] = d[u] + edge.weight;
                p[edge.neighbor] = u;
                unvisited.push({d[u] + edge.weight, edge.neighbor});
            }
        }
    }

    double totalDistance = d[dest];

    // Contruct Shortest Path from Predecessor (p) Map
    std::vector<std::string> shortestPath;
    std::string curr = dest;
    while (!curr.empty()) {
        shortestPath.push_back(curr);
        if (curr == src)
            break;
        curr = p[curr];
    }

    if(shortestPath.back() != src)
        return {{}, 0.0};
    std::reverse(shortestPath.begin(), shortestPath.end());
    return {shortestPath, totalDistance};
}
