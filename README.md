# Cross Campus Quickly

## Project Overview
Cross Campus Quickly is a program that finds the shortest walking path from point A on the UF campus to another point B on the UF campus using the breadth-first search algorithm and Dijkstra's algorithm. 

## Features
- Map display with selectable locations/nodes
- Djikstra and Breadth First Search algorithms to determine paths
- Graph data structure representation of the campus
- Interactive side panel and window built with QT framework (coordinates, select algorithm, zoom in/out, line outlining path)

## Technology & Requirements
- C++ 17 or later
- Qt 6
- A compatible compiler/toolchain: MSVC 64-bit or MinGW (depending on your Qt install)
- CLion, Visual Studio, or Qt-Creator or other IDE
 
## Getting Started
  1. **Install Qt**
     - Download from https://www.qt.io/download-dev 
  2. **Clone the Repository**
  3. **Configure with CMake**
  4. **Build & Run**
     - When window pops up, click which algorithm you want, then a starting node and ending node to get a path
     - The map highlights the path
     - Use the mouse to zoom in/explore the map
     - Click again to clear or get another path
