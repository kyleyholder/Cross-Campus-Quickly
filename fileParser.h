#include <unordered_map>
#include <vector>
#include <QGraphicsScene>
#include <QPolygonF>
#include <QPointF>
#include <QBrush>
#include <QPen>
#include <iostream>
#include <fstream>

class FileParser{
public:
    struct Node {
        double lat;
        double lon;
    };

    void parse(std::string fileName, QGraphicsScene &scene);
    void parseNodes(std::string fileName);
    void parseWays(std::string fileName);
    void parseWaysViz(std::string fileName, QGraphicsScene &scene);


    const std::unordered_map<std::string, FileParser::Node>& getNodes() const;
    const std::unordered_map<std::string, std::vector<std::string>>& getWays() const;

private:
    std::unordered_map<std::string, Node> nodes;
    std::unordered_map<std::string, std::vector<std::string>> ways;
};