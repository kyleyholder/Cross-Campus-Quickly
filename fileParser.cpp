#include "fileParser.h"

void FileParser::parse(std::string fileName, QGraphicsScene &scene){
    parseNodes(fileName);
    parseWaysViz(fileName, scene);
}

const std::unordered_map<std::string, FileParser::Node>& FileParser::getNodes() const {
    return nodes;
}

const std::unordered_map<std::string, std::vector<std::string>>& FileParser::getWays() const{
    return ways;
}

void FileParser::parseNodes(std::string fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cout << "Error opening file" << fileName << std::endl;
    }

    std::string line;
    while (getline(file, line)) {
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

void FileParser::parseWays(std::string fileName){
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cout << "Error opening file " << fileName << std::endl;
        return;
    }

    std::string line;

    bool recordingWay = false;
    std::string currentWayId;
    std::vector<std::string> currentNodeRefs;

    while (getline(file, line)) {
        if (line.find("<way") != std::string::npos) {
            recordingWay = true;
            currentNodeRefs = {};

            int idStart = static_cast<int>(line.find("id=\""));
            if (idStart != std::string::npos) {
              idStart += 4;
              int idEnd = static_cast<int>(line.find("\"", idStart));
              currentWayId = line.substr(idStart, idEnd - idStart);
            }
        }

        if (recordingWay && line.find("<nd ref=\"") != std::string::npos) {
            int refStart = static_cast<int>(line.find("ref=\"")) + 5;
            int refEnd = static_cast<int>(line.find("\"", refStart));
            currentNodeRefs.push_back(line.substr(refStart, refEnd - refStart));
        }

        if (line.find("</way") != std::string::npos) {
          recordingWay = false;
          ways[currentWayId] = currentNodeRefs;
        }
    }
}

void FileParser::parseWaysViz(std::string fileName, QGraphicsScene &scene){
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

    while(getline(file, line)) {
        if (line.find("<way") != std::string::npos) {
            recordingWay = true;
            isRoad = false;
            isBuilding = false;
            isWater = false;
            isPark = false;
            isPool = false;
            currentNodeRefs = {};
        }

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
            if (isRoad) {
                QPen pen(Qt::white);
                for (int i = 1; i < static_cast<int>(currentNodeRefs.size()); ++i) {
                    Node n1 = nodes[currentNodeRefs[i-1]];
                    Node n2 = nodes[currentNodeRefs[i]];

                    if (n1.lat > 29.6356 && n1.lat < 29.6523 &&
                    n1.lon > -82.3725 && n1.lon < -82.3391 & n2.lat > 29.6356 && n2.lat < 29.6523 &&
                    n2.lon > -82.3725 && n2.lon < -82.3391) {
                        scene.addLine((((((n2.lon+82)*1000))+391)*29), 1002-((((n2.lat-29)*1000)-356)*29), (((((n1.lon+82)*1000))+391)*29), 1002-((((n1.lat-29)*1000)-356)*29), pen);
                    }
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

                QPen pen(Qt::blue);
                QBrush brush(Qt::blue);

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