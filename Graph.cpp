//
// Created by secga on 10/31/2025.
//

#include "Graph.h"
#include <cmath>

Graph::Graph(const std::unordered_map<std::string, FileParser::Node>& nodes,
             const std::unordered_map<std::string, std::vector<std::string>>& ways){

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

const std::unordered_map<std::string, std::vector<Edge>>& Graph::getAdjacencyList() const{
     return adjacencyList;
}

double Graph::calculateDistance(const FileParser::Node& node1, const FileParser::Node& node2){
  double xDistance = node1.lon - node2.lon;
  double yDistance = node1.lat - node2.lat;
  return std::sqrt((xDistance * xDistance) + (yDistance * yDistance));
}
