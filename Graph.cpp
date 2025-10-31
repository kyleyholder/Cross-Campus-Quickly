//
// Created by secga on 10/31/2025.
//

#include "Graph.h"
#include <cmath>

Graph::Graph(const std::unordered_map<std::string, FileParser::Node>& nodes,
             const std::unordered_map<std::string, std::vector<std::string>>& ways){

    for (const auto& [wayID, nodeIDs] : ways){
      for (size_t i = 1; i < nodeIDs.size(); i++){
        const std::string& start = nodeIDs[i - 1];
        const std::string& end = nodeIDs[i];

        if (nodes.count(start) > 0 && nodes.count(end) > 0){
          const FileParser::Node& node1 = nodes.at(start);
          const FileParser::Node& node2 = nodes.at(end);

          double dist = calculateDistance(node1, node2);

          adjacencyList[start].push_back({start, dist});
          adjacencyList[end].push_back({end, dist});
        }
      }
    }
}

const std::unordered_map<std::string, std::vector<Edge>>& Graph::getAdjacencyList() const{
     return adjacencyList;
}
