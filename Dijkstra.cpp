/*
* Dijkstra.cpp
* Author: Shelby Coonce
* Description: File containing Dijkstra algorithm for finding the shortest paths between nodes in a weighted graph
* Date: November 3, 2025
*/

#include "Dijkstra.h"

std::vector<std::string> Dijkstra(const Graph& graph, const std::string& src, const std::string& dest) {
    const std::unordered_map<std::string, std::vector<Edge>>& adj = graph.getAdjacencyList();

    if(!adj.count(src) || !adj.count(dest)) {
        return {};
    }

    if(src == dest) {
        return {src};
    }

    // Visited and Unvisited Sets
    std::unordered_set<std::string> visited;
    std::set<std::pair<double, std::string>> unvisited;
    unvisited.insert({0, src});

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
        std::string u = unvisited.begin()->second;
        unvisited.erase(unvisited.begin());

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
                unvisited.insert({d[u] + edge.weight, edge.neighbor});
            }
        }
    }

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
        return {};
    std::reverse(shortestPath.begin(), shortestPath.end());

    return shortestPath;
}