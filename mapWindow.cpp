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

    labelLatitudeStart = new QLabel("", panel);
    labelLongitudeStart = new QLabel("", panel);
    labelLatitudeDestination = new QLabel("", panel);
    labelLongitudeDestination = new QLabel("", panel);
    labelErrorMessage = new QLabel("", panel);
    labelDistance = new QLabel("", panel);
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
    QLabel *sectionLabelShortestPathAlgorithm = new QLabel("SHORTEST PATH ALGORITHM");
    QLabel *sectionLabelStartingPoint = new QLabel("STARTING POINT:");
    QLabel *sectionLabelStartingPointLongitude = new QLabel("LONGITUDE:");
    QLabel *sectionLabelStartingPointLatitude = new QLabel("LATITUDE:");
    QLabel *sectionLabelDestinationLongitude = new QLabel("LONGITUDE:");
    QLabel *sectionLabelDestinationLatitude = new QLabel("LATITUDE:");
    QLabel *sectionLabelDestination = new QLabel("DESTINATION:");
    QLabel *sectionLabelDistanceOfPath = new QLabel("DISTANCE OF PATH");

    //Setting font size and color for labels
    sectionLabelShortestPathAlgorithm->setStyleSheet("color: white; font-size: 16px;");
    sectionLabelStartingPoint->setStyleSheet("color: #3e7fc9; font-size: 16px;");
    sectionLabelDestination->setStyleSheet("color: red; font-size: 16px;");
    sectionLabelDistanceOfPath->setStyleSheet("color: white; font-size: 16px;");
    sectionLabelStartingPointLongitude->setStyleSheet("color: white;  font-size: 14px;");
    sectionLabelStartingPointLatitude->setStyleSheet("color: white;  font-size: 14px;");
    sectionLabelDestinationLongitude->setStyleSheet("color: white;  font-size: 14px;");
    sectionLabelDestinationLatitude->setStyleSheet("color: white;  font-size: 14px;");
    labelLongitudeStart->setStyleSheet("color: white; background-color: grey; font-size: 16px;");
    labelLatitudeStart->setStyleSheet("color: white; background-color: grey; font-size: 16px;");
    labelLongitudeDestination->setStyleSheet("color: white; background-color: grey; font-size: 16px;");
    labelLatitudeDestination->setStyleSheet("color: white; background-color: grey; font-size: 16px;");
    labelDistance->setStyleSheet("color: white; background-color: grey; font-size: 16px;");
    labelErrorMessage->setStyleSheet("color: red; font-size: 16px;");

    //Adding widgets(labels, buttons, combo boxe, etc.) to the panelLayout
    panelLayout->addWidget(sectionLabelStartingPoint);
    panelLayout->addWidget(sectionLabelStartingPointLongitude);
    panelLayout->addWidget(labelLongitudeStart);
    panelLayout->addWidget(sectionLabelStartingPointLatitude);
    panelLayout->addWidget(labelLatitudeStart);
    panelLayout->addWidget(sectionLabelDestination);
    panelLayout->addWidget(sectionLabelDestinationLongitude);
    panelLayout->addWidget(labelLongitudeDestination);
    panelLayout->addWidget(sectionLabelDestinationLatitude);
    panelLayout->addWidget(labelLatitudeDestination);
    panelLayout->addWidget(sectionLabelShortestPathAlgorithm);
    panelLayout->addWidget(comboBox);
    panelLayout->addWidget(comboBox);
    panelLayout->addWidget(sectionLabelDistanceOfPath);
    panelLayout->addWidget(labelDistance);
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
        labelLatitudeStart->setText(g);

        QString c = QString::number(x);
        labelLongitudeStart->setText(c);

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
        labelLatitudeDestination->setText(g);

        QString c = QString::number(x);
        labelLongitudeDestination->setText(c);

        secondClickNodeID = graph.findClosestNode(y, x, parser.nodes);

        FileParser::Node node = parser.nodes[secondClickNodeID];

        double xPosition = (((node.lon + 82) * 1000) + 391) * 29;
        double yPosition = 1002 - ((((node.lat - 29) * 1000) - 356) * 29);

        point2 = scene.addEllipse(xPosition - 5, yPosition - 5, 12, 12, QPen(Qt::red), QBrush(Qt::red));


        QString str = "Dijkstra's Algorithm";
        std::string str1 = selectedAlgorithm.toStdString();

        if (selectedAlgorithm == str) {
            std::pair<std::vector<std::string>, double> DijkstraOutputPair = Dijkstra(graph, firstClickNodeID, secondClickNodeID);

            if (DijkstraOutputPair.first.size() == 0) {
                labelErrorMessage->setText("There is no path between the points.");
            }

            std::string distance = std::to_string(63 * DijkstraOutputPair.second) + " mi"; //multiplying by 63 to convert to miles
            QString distanceQString = QString::fromStdString(distance);

            if (DijkstraOutputPair.first.size() != 0) {
                labelDistance->setText(distanceQString);
            }

            //Adding path to the scene in the loop
            QPen pen(QColor(255, 102, 0, 255), 3);
            for (int i = 1; i < static_cast<int>(DijkstraOutputPair.first.size()); ++i) {
                FileParser::Node n1 = parser.nodes[DijkstraOutputPair.first[i-1]];
                FileParser::Node n2 = parser.nodes[DijkstraOutputPair.first[i]];

                QGraphicsLineItem* line = scene.addLine((((((n2.lon+82)*1000))+391)*29), 1002-((((n2.lat-29)*1000)-356)*29), (((((n1.lon+82)*1000))+391)*29), 1002-((((n1.lat-29)*1000)-356)*29), pen);
                pathLines.push_back(line);
            }
        }else{
            std::pair<std::vector<std::string>, double> bfsOutputPair = bfs(graph, firstClickNodeID, secondClickNodeID);

            if (bfsOutputPair.first.size() == 0) {
                labelErrorMessage->setText("There is no path between the points.");
            }

            std::string distance = std::to_string(63 * bfsOutputPair.second) + " mi"; //multiplying by 63 to convert to miles
            QString distanceQString = QString::fromStdString(distance);

            if (bfsOutputPair.first.size() != 0) {
                labelDistance->setText(distanceQString);
            }

            //Adding path to the scene in the loop
            QPen pen(QColor(255, 102, 0, 255), 3);
            for (int i = 1; i < static_cast<int>(bfsOutputPair.first.size()); ++i) {
                FileParser::Node n1 = parser.nodes[bfsOutputPair.first[i-1]];
                FileParser::Node n2 = parser.nodes[bfsOutputPair.first[i]];

                QGraphicsLineItem* line = scene.addLine((((((n2.lon+82)*1000))+391)*29), 1002-((((n2.lat-29)*1000)-356)*29), (((((n1.lon+82)*1000))+391)*29), 1002-((((n1.lat-29)*1000)-356)*29), pen);
                pathLines.push_back(line);
            }
        }
        //Third click, reset the path and the coordinates
    }else{
        firstClickStatus = false;
        secondClickStatus = false;

        labelLatitudeStart->setText("");
        labelLongitudeStart->setText("");
        labelLatitudeStart->setText("");
        labelLongitudeDestination->setText("");
        labelErrorMessage->setText("");
        labelDistance->setText("");


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
