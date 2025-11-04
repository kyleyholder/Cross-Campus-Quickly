/*
* fileParser.h
* Author: Artyom Chvanko
* Description: Header file for FileParser class
* Date: November 4, 2025
*/

#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <unordered_map>
#include <QGraphicsScene>
#include <QPolygonF>
#include <QPointF>
#include <QBrush>
#include <QPen>
#include <iostream>
#include <fstream>

class FileParser{
public:
    //Node struct for storage of latitude and longitude pairs
    struct Node {
        double lat;
        double lon;
    };

    void parse(std::string fileName, QGraphicsScene &scene);
    void parseNodes(std::string fileName);
    void parseWays(std::string fileName, QGraphicsScene &scene);

    const std::unordered_map<std::string, FileParser::Node>& getNodes() const;


    std::unordered_map<std::string, Node> nodes;
    std::unordered_map<std::string, std::vector<std::string>> roads;
private:

};

#endif