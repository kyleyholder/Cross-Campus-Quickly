/*
* Graph.cpp
* Author: Shelby Coonce
* Description: Definition of Graph class which implements adjacency list for storage of the map nodes and paths. Also includes calculateDistance and findClosestNode methods
* Date: November 4, 2025
*/

#include "Graph.h"
#include <cmath>

//Definition of default constructor
Graph::Graph(){

}

//Parameterized constructor
Graph::Graph(const std::unordered_map<std::string, FileParser::Node>& nodes, const std::unordered_map<std::string, std::vector<std::string>>& ways){
    for (const auto& [wayID, nodeIDs] : ways){
        for (size_t i = 1; i < nodeIDs.size(); i++){
            const std::string& from = nodeIDs[i - 1];
            const std::string& to = nodeIDs[i];

            if (nodes.count(from) > 0 && nodes.count(to) > 0){
                const FileParser::Node& node1 = nodes.at(from);
                const FileParser::Node& node2 = nodes.at(to);

                double dist = calculateDistance(node1, node2);

                adjacencyList[from].push_back({to, dist});
                adjacencyList[to].push_back({from, dist});
            }
        }
    }
}

//Getter for adjacency list
const std::unordered_map<std::string, std::vector<Edge>>& Graph::getAdjacencyList() const{
    return adjacencyList;
}

//calculateDistance method
double Graph::calculateDistance(const FileParser::Node& node1, const FileParser::Node& node2){
    double xDistance = node1.lon - node2.lon;
    double yDistance = node1.lat - node2.lat;
    return std::sqrt((xDistance * xDistance) + (yDistance * yDistance));
}

//Method for finding the closest road node in a graph to given latitude and longitude
std::string Graph::findClosestNode(double latitude, double longitude, const std::unordered_map<std::string, FileParser::Node>& allNodes) const {
    double minDistance = std::numeric_limits<double>::max();
    std::string closestNodeID;

    for (const auto& [id, edges] : adjacencyList) {
        if (edges.empty()) {
            continue;
        }
        auto it = allNodes.find(id);
        if (it == allNodes.end()) continue;

        const auto& n = it->second;
        double dx = n.lon - longitude;
        double dy = n.lat - latitude;
        double dist = std::sqrt(dx*dx + dy*dy);

        if (dist < minDistance) {
            minDistance = dist;
            closestNodeID = id;
        }
    }
    return closestNodeID;
}