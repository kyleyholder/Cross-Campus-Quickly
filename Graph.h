/*
* Graph.h
* Author: Shelby Coonce
* Description: Header file for Graph class including declarations
* Date: November 4, 2025
*/

#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <vector>
#include <string>
#include "fileParser.h"

struct Edge {
    std::string neighbor;
    double weight;
};



class Graph {
private:
    // Adjacency List: node ID -> vector of neibors ID's & distances
    std::unordered_map<std::string, std::vector<Edge>> adjacencyList;

    // Returns Distance Between Nodes
    double calculateDistance(const FileParser::Node& node1, const FileParser::Node& node2);

public:
    Graph();
    // Constructor
    Graph(const std::unordered_map<std::string, FileParser::Node>& nodes,
          const std::unordered_map<std::string, std::vector<std::string>>& ways);

    // Returns Adjacency List
    const std::unordered_map<std::string, std::vector<Edge>>& getAdjacencyList() const;

    std::string findClosestNode(double latitude, double longitude,
                                   const std::unordered_map<std::string, FileParser::Node>& allNodes) const;

};

#endif //GRAPH_H