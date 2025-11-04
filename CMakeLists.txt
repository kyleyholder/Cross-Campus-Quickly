cmake_minimum_required(VERSION 3.30)
project(project2)

set(CMAKE_CXX_STANDARD 14)


# Point to your Qt installation
set(CMAKE_PREFIX_PATH "/Users/artyomchvanko/Qt/6.9.3/macos/lib/cmake/QT6")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(project2
        main.cpp
        mapWindow.cpp
        mapWindow.h
        fileParser.cpp
        fileParser.h
        bfs.cpp
        Graph.cpp
        Graph.h
        Dijkstra.cpp
        Dijkstra.h
        bfs.cpp
        bfs.h
)
target_link_libraries(project2 PRIVATE Qt6::Widgets)



