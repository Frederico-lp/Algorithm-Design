//
// Created by Rodrigo on 07/05/2021.
//

#include "Node.h"

Node::Node(int id,double xPos,double yPos){
    this->id = id;
    this->xPos = xPos;
    this->yPos = yPos;
}

Node::Node(int id){
    this->id = id;
}

Node::Node(){}

int Node::getId() const {
    return id;
}

void Node::setId(int id) {
    Node::id = id;
}

double Node::getxPos() const {
    return xPos;
}

void Node::setxPos(double xPos) {
    Node::xPos = xPos;
}

double Node::getyPos() const {
    return yPos;
}

void Node::setyPos(double yPos) {
    Node::yPos = yPos;
}


