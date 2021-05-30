//
// Created by Rodrigo on 07/05/2021.
//

#include "LoadFiles.h"

void LoadFiles::loadVertexes(Graph<Node> &g, GraphViewer &gv) {
    string line;
    ifstream file ("../files/espinho_full_nodes_xy.txt");


    if (file.is_open()) {  //opens file

        cout.precision(10);

        getline(file, line); //gets numNodes

        stringstream geek(line); //convert string to int
        int numNodes;
        geek >> numNodes;
        cout << "Reading Nodes ";

        for(int i=0;i<numNodes;i++){
            if(i == 6000 || i == 12000 || i == 17000)
                cout << ". ";

            getline(file, line); //gets each node line

            int index=1;
            vector<string> info;

            for(int j=1;j<line.length();j++){
                if (line[j] == ',' || line[j] == ')') {
                    info.push_back(line.substr(index, j - index));
                    index = j + 1;
                }
            }

            int nodeId = stoi(info[0]);

            double xPos = stod(info[1]);
            double yPos = stod(info[2]);

            Node currNode(nodeId,xPos,yPos);

           g.addVertex(currNode);
           gv.addNode(nodeId, sf::Vector2f(xPos, -yPos));
        }
        cout << endl;
        file.close();
    }
    else cerr<<"Error opening vertexes file\n";
}


void LoadFiles::loadEdges(Graph<Node> &g, GraphViewer &gv){
    string line;
    ifstream file ("../files/espinho_full_edges.txt");

    if (file.is_open()) {  //opens file

        cout.precision(10);

        getline(file, line); //gets numNodes

        stringstream geek(line); //convert string to int
        int numNodes;
        geek >> numNodes;

        cout << "Reading Edges ";

        for(int i=0; i<numNodes; i++){
            getline(file, line);

            if(i == 7000 || i == 14000 || i == 18500)
                cout << ". ";

            int index=1;
            vector<string> info;

            for(int j=1;j<line.length();j++){
                if (line[j] == ',' || line[j] == ')') {
                    info.push_back(line.substr(index, j - index));
                    index = j + 1;
                }
            }

            int firstID = stod(info[0]);
            int secondID = stod(info[1]);

            Node source = g.getVertexSet().at(firstID-1)->getInfo();
            Node dest = g.getVertexSet().at(secondID-1)->getInfo();
            double weight = getDistance(source.getxPos(), dest.getxPos(), source.getyPos(), dest.getyPos());
            g.addEdge(source, dest, weight);
            gv.addEdge(i,gv.getNode(firstID), gv.getNode(secondID), GraphViewer::Edge::EdgeType::DIRECTED);

        }
        cout << endl;
        file.close();
    }
    else cerr<<"Error opening edges file\n";

}

void LoadFiles::loadTags(Graph<Node> &g, GraphViewer &gv) {
    string line;
    ifstream file ("../files/espinho_tags.txt");

    if (file.is_open()) {  //opens file

        cout.precision(10);

        getline(file, line); //gets numType

        stringstream s(line); //convert string to int
        int numTypes;
        s >> numTypes;
        cout << "Reading Tags ";

        for (int i = 0; i<numTypes;i++){

            cout << ". ";
            getline(file, line); //gets numType
            string type = line;

            getline(file, line); //gets numType

            stringstream s(line); //convert string to int
            int numEntries;
            s >> numEntries;

            if( type == "parque") {
                for (int j=0;j<numEntries;j++){
                    getline(file, line); //gets each node line

                    int index=1;
                    vector<string> info;

                    for(int j=1;j<line.length();j++){
                        if (line[j] == ',' || line[j] == ')') {
                            info.push_back(line.substr(index, j - index));
                            index = j + 1;
                        }
                    }

                    int nodeId = stoi(info[0]);

                    double fixedPrice = stod(info[1]);
                    double hourlyPrice = stod(info[2]);

                    Node currNode(nodeId,0,0);

                    parking.push_back(currNode);
                    prices.push_back(make_pair(fixedPrice,hourlyPrice));
                    GraphViewer::Node& gvNode = gv.getNode(nodeId);
                    gvNode.setColor(GraphViewer::BLUE);
                    gvNode.setLabel("Park");


                    }
            }
            else if (type == "bomba"){
                for (int j=0;j<numEntries;j++) {
                    getline(file, line); //gets each node line
                    int nodeId = stoi(line);
                    Node currNode(nodeId,0,0);
                    bombas.push_back(currNode);
                    GraphViewer::Node& gvNode = gv.getNode(nodeId);
                    gvNode.setColor(GraphViewer::YELLOW);
                    gvNode.setLabel("Gas station");
                }
            }
            else if (type == "farmacia"){
                for (int j=0;j<numEntries;j++) {
                    getline(file, line); //gets each node line
                    int nodeId = stoi(line);
                    Node currNode(nodeId,0,0);
                    farmacias.push_back(currNode);
                    GraphViewer::Node& gvNode = gv.getNode(nodeId);
                    gvNode.setColor(GraphViewer::PINK);
                    gvNode.setLabel("Pharmacy");

                }
            }
            else if (type == "cafe"){
                for (int j=0;j<numEntries;j++) {
                    getline(file, line); //gets each node line
                    int nodeId = stoi(line);
                    Node currNode(nodeId,0,0);
                    cafes.push_back(currNode);
                    GraphViewer::Node& gvNode = gv.getNode(nodeId);
                    gvNode.setColor(GraphViewer::CYAN);
                    gvNode.setLabel("Coffee");
                }
            }
            else cout << "\n\nError\n\n";

        }
        cout << endl;
        file.close();
    }
}

void LoadFiles::loadStrong(Graph<Node> &g, GraphViewer &gv) {
    string line;
    ifstream file ("../files/espinho_strong_nodes_latlng.txt");


    if (file.is_open()) {  //opens file

        cout.precision(10);

        getline(file, line); //gets numNodes

        stringstream geek(line); //convert string to int
        int numNodes;
        geek >> numNodes;
        cout << "Reading Strong Nodes ";

        for(int i=0;i<numNodes;i++){
            if(i == 2000 || i == 4500 || i == 7000)
                cout << ". ";

            getline(file, line); //gets each node line

            int index=1;

            int nodeId;
            for(int j=1;j<line.length();j++){
                if (line[j] == ',' || line[j] == ')') {
                    nodeId = stoi(line.substr(index, j - index));
                    break;
                }
            }
            strong.push_back(Node(nodeId));
        }
        cout << endl;
        file.close();
    }
    else cerr<<"Error opening vertexes file\n";
}



double LoadFiles::getDistance(double x1, double x2, double y1, double y2) {

    return sqrt(pow((x1-x2),2) + pow((y1-y2),2));
}

const vector<Node> &LoadFiles::getParking() const {
    return parking;
}

const vector<pair<double, double>> &LoadFiles::getPrices() const {
    return prices;
}

const vector<Node> &LoadFiles::getCafes() const {
    return cafes;
}

const vector<Node> &LoadFiles::getBombas() const {
    return bombas;
}

const vector<Node> &LoadFiles::getFarmacias() const {
    return farmacias;
}

const vector<Node> &LoadFiles::getStrong() const {
    return strong;
}

