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
    // Adjacency List: node ID -> vector of neibors ID's & distances
    std::unordered_map<std::string, std::vector<Edge>> adjacencyList;

    double calculateDistance(const FileParser::Node& node1, const FileParser::Node& node2);

  public:
    Graph(const std::unordered_map<std::string, FileParser::Node>& nodes,
          const std::unordered_map<std::string, std::vector<std::string>>& ways);

    const std::unordered_map<std::string, std::vector<Edge>>& getAdjacencyList() const;

}

#endif //GRAPH_H
