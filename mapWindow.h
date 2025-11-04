/*
* mapWindow.h
* Author: Artyom Chvanko
* Description: Header file for mapWindow class including declarations
* Date: November 4, 2025
*/

#ifndef MAPWINDOW_H
#define MAPWINDOW_H

// Included Qt libraries
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QMouseEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

//Project header files
#include "bfs.h"
#include "fileParser.h"
#include "Graph.h"
#include "Dijkstra.h"

//C++ standard headers
#include <iostream>
#include <algorithm>
#include <fstream>

class MapWindow : public QGraphicsView {
public:
    //Constructor
    MapWindow(std::string filename, QWidget* parent = nullptr);

    //Destructor
    ~MapWindow();

    //Method for setting up the main window
    void drawWindow(std::string filename);
protected:
    //Method for user click event on the map
    void mousePressEvent(QMouseEvent *event) override;
private:
    QString selectedAlgorithm;
    QGraphicsScene scene;
    QWidget *panel ;
    QVBoxLayout *panelLayout;
    FileParser parser;
    Graph graph;

    //Labels for latitude and longitude of starting point and destination
    QLabel *labelLatitude1;
    QLabel *labelLongitude1;
    QLabel *labelLatitude2;
    QLabel *labelLongitude2;
    QLabel *labelErrorMessage;
};
#endif //MAPWINDOW_H
