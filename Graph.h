//
// Created by secga on 10/31/2025.
//

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
    // Constructor
    Graph(const std::unordered_map<std::string, FileParser::Node>& nodes,
          const std::unordered_map<std::string, std::vector<std::string>>& ways);

    // Returns Adjacency List
    const std::unordered_map<std::string, std::vector<Edge>>& getAdjacencyList() const;

};

#endif //GRAPH_H
