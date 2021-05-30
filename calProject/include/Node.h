//
// Created by Rodrigo on 07/05/2021.
//

#ifndef CALPROJECT_NODE_H
#define CALPROJECT_NODE_H


class Node {
private:
    int id;
    double xPos;
    double yPos;
public:
    Node(int id,double xPos,double yPos);

    Node(int id);

    Node();

    int getId() const;

    void setId(int id);

    double getxPos() const;

    void setxPos(double xPos);

    double getyPos() const;

    void setyPos(double yPos);

    bool operator==(const Node& n2){
        return this->id == n2.getId();
    }
    bool operator!=(const Node& n2){
        return this->id != n2.getId();
    }

};



#endif //CALPROJECT_NODE_H
