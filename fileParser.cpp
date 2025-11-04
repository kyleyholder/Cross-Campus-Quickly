/*
* fileParser.cpp
* Author: Artyom Chvanko
* Description: Definition of FileParser class which performs file reading and adding the elements from the data file to the map
* Date: November 4, 2025
*/

#include "fileParser.h"

//Main parse method that calls two parser methods: parseNodes, parseWays
void FileParser::parse(std::string fileName, QGraphicsScene &scene){
    parseNodes(fileName);
    parseWays(fileName, scene);
}

//Method for returning nodes
const std::unordered_map<std::string, FileParser::Node>& FileParser::getNodes() const {
    return nodes;
}

//Parsing file and adding nodes to the map
void FileParser::parseNodes(std::string fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cout << "Error opening file" << fileName << std::endl;
    }

    std::string line;
    while (getline(file, line)) {
        //If <node is found in the file, begin reading
        if (line.find("<node") != std::string::npos) {
            int idStart = static_cast<int>(line.find("id=\""));
            int latitudeStart = static_cast<int>(line.find("lat=\""));
            int longitudeStart = static_cast<int>(line.find("lon=\""));

            if (idStart != -1 && latitudeStart != -1 && longitudeStart != -1) {
                idStart += 4;
                latitudeStart += 5;
                longitudeStart += 5;

                int idEnd = static_cast<int>(line.find("\"", idStart));
                int latitudeEnd = static_cast<int>(line.find("\"", latitudeStart));
                int longitudeEnd = static_cast<int>(line.find("\"", longitudeStart));

                if (idEnd != -1 && latitudeEnd != -1 && longitudeEnd != -1) {
                    std::string id = line.substr(idStart, idEnd - idStart);
                    std::string latStr = line.substr(latitudeStart, latitudeEnd - latitudeStart);
                    std::string lonStr = line.substr(longitudeStart, longitudeEnd - longitudeStart);

                    if (!latStr.empty() && !lonStr.empty() && !lonStr.empty()) {
                        double latitude = std::stod(latStr);
                        double longitude = std::stod(lonStr);

                        nodes[id] = Node{latitude, longitude};
                    }
                }
            }
        }
    }
}

//Parsing ways and adding their visualization to the scene
void FileParser::parseWays(std::string fileName, QGraphicsScene &scene){
    std::vector<std::string> nodeRefs;

    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cout << "Error opening file " << fileName << std::endl;
    }

    std::string line;

    bool recordingWay = false;
    bool isRoad = false;
    bool isBuilding = false;
    bool isWater = false;
    bool isPark = false;
    bool isPool = false;
    std::vector<std::string> currentNodeRefs;
    std::string currentWayId;

    while(getline(file, line)) {
        //If <way is found in the file, begin reading
        if (line.find("<way") != std::string::npos) {
            recordingWay = true;
            isRoad = false;
            isBuilding = false;
            isWater = false;
            isPark = false;
            isPool = false;
            currentNodeRefs = {};

            int idStart = static_cast<int>(line.find("id=\""));
            if (idStart != std::string::npos) {
                idStart += 4;
                int idEnd = static_cast<int>(line.find("\"", idStart));
                currentWayId = line.substr(idStart, idEnd - idStart);
            }
        }

        //Determining the category of the current way
        if (recordingWay && line.find("<tag k=\"building\"") != std::string::npos) {
            isBuilding = true;
        }

        if (recordingWay && line.find("<tag k=\"highway\"") != std::string::npos) {
            isRoad = true;
        }

        if (recordingWay && line.find("<tag k=\"water\"") != std::string::npos) {
            isWater = true;
        }

        if (recordingWay && line.find("<tag k=\"leisure\"") != std::string::npos) {
            if (recordingWay && line.find("<tag k=\"leisure\" v=\"swimming_pool\"")!= std::string::npos) {
                isPool = true;
            }else{
                isPark = true;
            }
        }

        if (recordingWay && line.find("<nd ref=\"") != std::string::npos) {
            int refStart = static_cast<int>(line.find("ref=\"")) + 5;
            int refEnd = static_cast<int>(line.find("\"", refStart));
            currentNodeRefs.push_back(line.substr(refStart, refEnd - refStart));
        }


        if (line.find("</way") != std::string::npos) {
            QPolygonF polygon;
            //Adding elements of the map to the scene
            if (isRoad) {
                QPen pen(Qt::white);
                for (int i = 1; i < static_cast<int>(currentNodeRefs.size()); ++i) {
                    Node n1 = nodes[currentNodeRefs[i-1]];
                    Node n2 = nodes[currentNodeRefs[i]];


                    scene.addLine((((((n2.lon+82)*1000))+391)*29), 1002-((((n2.lat-29)*1000)-356)*29), (((((n1.lon+82)*1000))+391)*29), 1002-((((n1.lat-29)*1000)-356)*29), pen);


                    roads[currentWayId] = currentNodeRefs;
                }

            }else if (isBuilding) {
                for (const auto& nodeId : currentNodeRefs) {
                    Node n = nodes[nodeId];

                    double  x = (((n.lon + 82) * 1000) + 391) * 29;
                    double  y = 1002 - ((((n.lat - 29) * 1000) - 356) * 29);
                    polygon << QPointF(x, y);

                }

                QPen pen(QColor(0, 0, 0, 0));
                QBrush brush(Qt::gray);

                scene.addPolygon(polygon, pen, brush);
            }else if (isWater) {
                for (const auto& nodeId : currentNodeRefs) {
                    Node n = nodes[nodeId];

                    double x = (((n.lon + 82) * 1000) + 391) * 29;
                    double y = 1002 - ((((n.lat - 29) * 1000) - 356) * 29);
                    polygon << QPointF(x, y);
                }

                QPen pen(QColor(0,153,255, 0));
                QBrush brush(QColor(0,153,255));

                scene.addPolygon(polygon, pen, brush);
            }else if (isPark) {
                for (const auto& nodeId : currentNodeRefs) {
                    Node n = nodes[nodeId];

                    double x = (((n.lon + 82) * 1000) + 391) * 29;
                    double y = 1002 - ((((n.lat - 29) * 1000) - 356) * 29);
                    polygon << QPointF(x, y);

                }

                QBrush brush(Qt::green);
                QPen pen(QColor(0, 0, 0, 0));

                scene.addPolygon(polygon, pen, brush);
            }else if (isPool) {
                for (const auto& nodeId : currentNodeRefs) {
                    Node n = nodes[nodeId];

                    double x = (((n.lon + 82) * 1000) + 391) * 29;
                    double y = 1002 - ((((n.lat - 29) * 1000) - 356) * 29);
                    polygon << QPointF(x, y);

                }

                QBrush brush(QColor(144,213,255));
                QPen pen(QColor(0, 0, 0, 0));

                scene.addPolygon(polygon, pen, brush);
            }
            recordingWay = false;
        }
    }
}