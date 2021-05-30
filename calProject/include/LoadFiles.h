//
// Created by Rodrigo on 07/05/2021.
//

#ifndef CALPROJECT_LOADFILES_H
#define CALPROJECT_LOADFILES_H
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Node.h"
#include "Graph.h"
#include <graphviewer.h>

using namespace std;

class LoadFiles {

public:
    void loadVertexes(Graph<Node> &g, GraphViewer &gv);
    void loadStrong(Graph<Node> &g,GraphViewer &gv);
    void loadEdges(Graph<Node> &g, GraphViewer &gv);
    void loadTags(Graph<Node> &g, GraphViewer &gv);

    const vector<Node> &getParking() const;

    const vector<pair<double, double>> &getPrices() const;

    const vector<Node> &getCafes() const;

    const vector<Node> &getBombas() const;

    const vector<Node> &getFarmacias() const;

private:
    vector<Node> parking;
    vector<Node> strong;
public:
    const vector<Node> &getStrong() const;

private:
    vector<pair<double,double>> prices;
    vector<Node> cafes;
    vector<Node> bombas;
    vector<Node> farmacias;


    double getDistance(double x1,double x2,double y1,double y2);

};


#endif //CALPROJECT_LOADFILES_H
