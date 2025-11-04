/*
* main.cpp
* Description: Main function of the project
*/

#include <iostream>
#include <filesystem>
#include "mapWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MapWindow window("../map.osm");
    window.drawWindow("../map.osm");
    window.show();

    return app.exec();
}