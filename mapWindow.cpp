/*
* mapWindow.cpp
* Author: Artyom Chvanko
* Description: mapWindow class implementation with methods for drawing the window and doing operations with it (mouse clicking, choosing the path searching algorithm and zooming in and out)
* Date: November 4, 2025
*/


#include "mapWindow.h"

//Constructor
MapWindow::MapWindow(std::string filename, QWidget* parent) : QGraphicsView(parent) {
    this->setScene(&scene);
    setMouseTracking(true);
    parser.parse(filename, scene);
    graph = Graph(parser.nodes, parser.roads);

    panel = new QWidget();
    panelLayout = new QVBoxLayout(panel);

    labelLatitude1 = new QLabel("", panel);
    labelLongitude1 = new QLabel("", panel);
    labelLatitude2 = new QLabel("", panel);
    labelLongitude2 = new QLabel("", panel);
    labelErrorMessage = new QLabel("", panel);
    selectedAlgorithm = "Dijkstra's Algorithm";
}

//Destructor
MapWindow::~MapWindow() {
        delete panel;
        //don't need to delete anything else because when we delete panel, all children of the parent such
        //as labels are automatically deleted by Qt
}

//Method for setting up the map window by parsing the given file, populating and decorating the scene
void MapWindow::drawWindow(std::string fileName) {
    panel->setStyleSheet("background-color: #232324");
    this->setStyleSheet("background-color: #3d3d40");


    //Button for zoom in
    QPushButton *zoomIn = new QPushButton("Zoom In");
    zoomIn->setStyleSheet("color: white; background-color: #59595c; font-size: 16px;");
    QObject::connect(zoomIn, &QPushButton::clicked, [=]() {
        this->scale(1.2, 1.2);
    });

    //Button for zoom out
    QPushButton *zoomOut = new QPushButton("Zoom Out");
    zoomOut->setStyleSheet("color: white; background-color: #59595c; font-size: 16px;");
    QObject::connect(zoomOut, &QPushButton::clicked, [=]() {
        this->scale(1/1.5, 1/1.5);
    });

    //Combo box for path-finding algorithm options
    QComboBox *comboBox = new QComboBox();
    comboBox->setStyleSheet("color: white; background-color: #59595c; font-size: 16px;");
    comboBox->addItem("Dijkstra's Algorithm");
    comboBox->addItem("Breadth First Search Algorithm");

    QLabel *label = new QLabel("SHORTEST PATH ALGORITHM");
    label->setStyleSheet("color: white; font-size: 16px;");

    QObject::connect(comboBox, &QComboBox::currentTextChanged, [this, label](const QString &text) {
        this->selectedAlgorithm = text;
    });

    std::string str2 = selectedAlgorithm.toStdString();


    //Creating labels for marking the components of the control panel
    QLabel *label1 = new QLabel("SHORTEST PATH ALGORITHM");
    //QLabel *labelDistanceOfPath = new QLabel("DISTANCE OF PATH");
    QLabel *labelStartingPoint = new QLabel("STARTING POINT:");
    QLabel *labelDestination = new QLabel("DESTINATION");

    //Setting font size and color for labels
    label1->setStyleSheet("color: white; font-size: 16px;");
    //labelDistanceOfPath->setStyleSheet("color: white; font-size: 16px;");
    labelStartingPoint->setStyleSheet("color: blue; font-size: 16px;");
    labelDestination->setStyleSheet("color: red; font-size: 16px;");
    labelLongitude1->setStyleSheet("color: white; background-color: grey; font-size: 16px;");
    labelLatitude1->setStyleSheet("color: white; background-color: grey; font-size: 16px;");
    labelLongitude2->setStyleSheet("color: white; background-color: grey; font-size: 16px;");
    labelLatitude2->setStyleSheet("color: white; background-color: grey; font-size: 16px;");
    labelErrorMessage->setStyleSheet("color: red; font-size: 16px;");

    //Adding widgets(labels, buttons, combo boxe, etc.) to the panelLayout
    panelLayout->addWidget(labelStartingPoint);
    panelLayout->addWidget(labelLongitude1);
    panelLayout->addWidget(labelLatitude1);
    panelLayout->addWidget(labelDestination);
    panelLayout->addWidget(labelLongitude2);
    panelLayout->addWidget(labelLatitude2);
    panelLayout->addWidget(label1);
    panelLayout->addWidget(comboBox);
    //panelLayout->addWidget(labelDistanceOfPath);
    panelLayout->addWidget(labelErrorMessage);
    panelLayout->addStretch(4);
    panelLayout->addWidget(zoomIn);
    panelLayout->addWidget(zoomOut);


    QWidget *central = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(central);

    //Adding the map view (this) and the control panel to the layout
    layout->addWidget(this);
    layout->addWidget(panel);

    central->setWindowTitle("Cross Campus Quick");
    central->setStyleSheet("background-color: #232324");
    central->resize(1400, 600);
    central->show();

    central->setLayout(layout);
    central->show();
}

//Method for user click event on the map. Converts two clicks' position
//of the user to the coordinates on the map and draws the path according to the chosen algorithm
void MapWindow::mousePressEvent(QMouseEvent *event)  {
    QPointF scenePos = mapToScene(event->pos());

    //Creating static variables to save state between mouse clicks
    static bool firstClickStatus = false;
    static bool secondClickStatus = false;
    static std::string firstClickNodeID;
    static std::string secondClickNodeID;
    static QGraphicsEllipseItem* point1 = nullptr;
    static QGraphicsEllipseItem* point2 = nullptr;
    static QGraphicsLineItem* line = nullptr;
    static std::vector<QGraphicsLineItem*> pathLines;


    if (!firstClickStatus) {
        firstClickStatus = true;
        double x = ((((scenePos.x()/29) - 391)/1000) - 82);
        double y = (((((1002.0 - scenePos.y()) / 29) + 356) / 1000) + 29);

        QString g = QString::number(y);
        labelLatitude1->setText(g);

        QString c = QString::number(x);
        labelLongitude1->setText(c);

        firstClickNodeID = graph.findClosestNode(y, x, parser.nodes);

        FileParser::Node node = parser.nodes[firstClickNodeID];

        double xPosition = (((node.lon + 82) * 1000) + 391) * 29;
        double yPosition = 1002 - ((((node.lat - 29) * 1000) - 356) * 29);

        point1 = scene.addEllipse(xPosition - 5, yPosition - 5, 12, 12, QPen(Qt::blue), QBrush(Qt::blue));

    }else if (!secondClickStatus){
        secondClickStatus = true;
        double x = ((((scenePos.x()/29) - 391)/1000) - 82);
        double y = (((((1002.0 - scenePos.y()) / 29) + 356) / 1000) + 29);


        QString g = QString::number(y);
        labelLatitude2->setText(g);

        QString c = QString::number(x);
        labelLongitude2->setText(c);

        secondClickNodeID = graph.findClosestNode(y, x, parser.nodes);

        FileParser::Node node = parser.nodes[secondClickNodeID];

        double xPosition = (((node.lon + 82) * 1000) + 391) * 29;
        double yPosition = 1002 - ((((node.lat - 29) * 1000) - 356) * 29);

        point2 = scene.addEllipse(xPosition - 5, yPosition - 5, 12, 12, QPen(Qt::red), QBrush(Qt::red));


        QString str = "Dijkstra's Algorithm";
        std::string str1 = selectedAlgorithm.toStdString();

        if (selectedAlgorithm == str) {
            std::vector<std::string> shortestPathToDraw = Dijkstra(graph, firstClickNodeID, secondClickNodeID);

            if (shortestPathToDraw.size() == 0) {
                labelErrorMessage->setText("There is no path between the points.");
            }

            //Adding path to the scene in the loop
            QPen pen(QColor(255, 102, 0, 255), 3);
            for (int i = 1; i < static_cast<int>(shortestPathToDraw.size()); ++i) {
                FileParser::Node n1 = parser.nodes[shortestPathToDraw[i-1]];
                FileParser::Node n2 = parser.nodes[shortestPathToDraw[i]];

                QGraphicsLineItem* line = scene.addLine((((((n2.lon+82)*1000))+391)*29), 1002-((((n2.lat-29)*1000)-356)*29), (((((n1.lon+82)*1000))+391)*29), 1002-((((n1.lat-29)*1000)-356)*29), pen);
                pathLines.push_back(line);
            }
        }else{
            std::vector<std::string> shortestPathToDraw = bfs(graph, firstClickNodeID, secondClickNodeID);

            if (shortestPathToDraw.size() == 0) {
                labelErrorMessage->setText("There is no path between the points.");
            }

            //Adding path to the scene in the loop
            QPen pen(QColor(255, 102, 0, 255), 3);
            for (int i = 1; i < static_cast<int>(shortestPathToDraw.size()); ++i) {
                FileParser::Node n1 = parser.nodes[shortestPathToDraw[i-1]];
                FileParser::Node n2 = parser.nodes[shortestPathToDraw[i]];

                QGraphicsLineItem* line = scene.addLine((((((n2.lon+82)*1000))+391)*29), 1002-((((n2.lat-29)*1000)-356)*29), (((((n1.lon+82)*1000))+391)*29), 1002-((((n1.lat-29)*1000)-356)*29), pen);
                pathLines.push_back(line);
            }
        }
        //Third click, reset the path and the coordinates
    }else{
        firstClickStatus = false;
        secondClickStatus = false;

        labelLatitude1->setText("");
        labelLongitude1->setText("");
        labelLatitude2->setText("");
        labelLongitude2->setText("");
        labelErrorMessage->setText("");


        if (line) {
            scene.removeItem(line);
            delete line;
            line = nullptr;
        }

        if (point1) {
            scene.removeItem(point1);
            delete point1;
            point1 = nullptr;
        }

        if (point2) {
            scene.removeItem(point2);
            delete point2;
            point2 = nullptr;
        }


        for (auto lineItem : pathLines) {
            if (lineItem) {
                scene.removeItem(lineItem);
                delete lineItem;
            }

        }
        pathLines.clear();
    }
    QGraphicsView::mousePressEvent(event);
}
