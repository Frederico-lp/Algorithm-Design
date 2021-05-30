#include <iostream>
#include <math.h>
#include <regex>
#include <graphviewer.h>
#include "../include/Graph.h"
#include "../include/Node.h"
#include "../include/LoadFiles.h"

using namespace std;

//paints the given path (not including 1st and last point)
void setPathColor(vector<Node> path, GraphViewer &gv, int color){
    for(int i = 1;i < path.size() - 1; i++){
        GraphViewer::Node& node = gv.getNode(path.at(i).getId());
        if(node.getLabel() != "")
            continue;
        if(color == 0)
            node.setColor(GraphViewer::GREEN);
        else if(color == 1)
            node.setColor(GraphViewer::ORANGE);

    }

}

//returns closest park in the graph
pair<int, int> getClosestPark(std::vector<Node> parks, Graph<Node> &graph){
    int id;
    int dist = -1;
    int min = INT32_MAX;
    for(auto park: parks) {
        auto vertex = graph.findVertex(park);
        if (vertex->getDist() < min) {
            min = vertex->getDist();
            id = vertex->getInfo().getId();
            dist = vertex->getDist();
        }
    }
    return make_pair(id, dist);
}

//returns closest park to the final destination
pair<int, int> getDestinationPark(std::vector<Node> parks,
                                  Node destination, Graph<Node> graph){


    double dist = INF;
    int id = -1;
    Vertex<Node>* dest = graph.findVertex(destination);
    for(auto &park: parks){
        double currDist=0;
        graph.dijkstraShortestPath(park);
        currDist = dest->getDist();
        if(currDist < dist){
            dist = currDist;
            id = park.getId();
        }
    }

    return make_pair(id, dist);

}

//finds cheapest park given the hours
void findCheapestPark(std::vector<Node> parks,std::vector<pair<double,double>> prices,int hours,vector<int> &bestParks){
    double bestPrice = INF;

    for (int i=0;i<parks.size();i++){
        double currPrice = prices[i].first + (prices[i].second*hours);
        if (currPrice<bestPrice){
             bestParks.clear();
             bestParks.push_back(parks[i].getId());
             bestPrice = currPrice;
        }
        else if (currPrice == bestPrice)
            bestParks.push_back(parks[i].getId());
    }

}


// returns the cheapest park, in case of a tie returns the closest one
pair<int, int> getCheapestPark(std::vector<Node> parks, Graph<Node> &graph,std::vector<pair<double,double>> prices){

    int hours;
    cout << "How many hours are you planning on parking?" << endl;
    cin >> hours;
    cout.precision(5);

    vector<int> bestParks;
    findCheapestPark(parks,prices,hours,bestParks);
    Vertex<Node>* bestPark;
    double minDist = INF;
    if (bestParks.size() == 1){
        auto vertex = graph.findVertex(Node(bestParks[0],0,0));
        return make_pair(vertex->getInfo().getId(), vertex->getDist());
    }
    else{
        cout << "All this parks will cost you the same:" << endl;
        for (auto &parkId:bestParks){
            auto vertex = graph.findVertex(Node(parkId,0,0));
            cout << "Park n:" << parkId << " is " << vertex->getDist() << "m away" << endl;
            if (vertex->getDist() < minDist){
                bestPark = vertex;
                minDist=vertex->getDist();
            }
        }
        return make_pair(bestPark->getInfo().getId(), bestPark->getDist());
    }

}

//returns closest gas station to the origin point
Node bestGasOption(vector<Node> gasStations, Node &origin, Graph<Node> &graph, GraphViewer &gv){

    Node gasStation;
    double gasDist = INF;

    //percorre o path e em cada path compara qual o cafe/farmacia/bomba de gasolina mais proximo
    //e depois guarda o main proximo
        graph.dijkstraShortestPath(origin);
        for(auto &it: gasStations){
            auto vertex = graph.findVertex(it);
            if(vertex->getDist() < gasDist){
                gasDist = vertex->getDist();
                gasStation = it;
            }
        }
    return gasStation;

}
//returns closest pharmacy to the origin point
Node bestPharmacyOption(vector<Node> pharmacies, Node &origin, Graph<Node> &graph, GraphViewer &gv){
    Node pharmacy;
    double pharmDist = INF;

    graph.dijkstraShortestPath(origin);
    for(auto &it: pharmacies){
        auto vertex = graph.findVertex(it);
        if(vertex->getDist() < pharmDist){
            pharmDist = vertex->getDist();
            pharmacy = it;
        }
    }
    return pharmacy;
}
//returns closest coffee to the origin point
Node bestCoffeeOption(vector<Node> coffeeShops, Node &origin, Graph<Node> &graph, GraphViewer &gv){
    Node coffee;
    double coffeeDist = INF;

    //percorre o path e em cada no compara qual o cafe/farmacia/bomba de gasolina mais proximo
    //e depois retorna o mais proximo de todos

    graph.dijkstraShortestPath(origin);
    for(auto &it: coffeeShops){
        auto vertex = graph.findVertex(it);
        if(vertex->getDist() < coffeeDist){
            coffeeDist = vertex->getDist();
            coffee = it;
        }
    }
    return coffee;

}


//finds closest business type, 0 if type 1 has a business closer to the origin, 1 if its type2
int findClosestBusiness(vector<Node> type1 , vector<Node> type2, Graph<Node> &graph,Node &origin){
    graph.dijkstraShortestPath(origin);
    double minDist = INF;
    bool updated = false;
    for(auto &x: type1){
        auto vert = graph.findVertex(x);
        if(vert->getDist() < minDist){
            minDist = vert->getDist();
        }
    }
    for(auto &x: type2){
        auto vert = graph.findVertex(x);
        if(vert->getDist() < minDist){
            updated = true;
            minDist = vert->getDist();
        }
    }
    if(updated)
        return 1;
    else return 0;
}
//finds closest business type, 0 if type 1 has a business closer to the origin, 1 if its type2,2 if its type 3
int findClosestBusiness(vector<Node> type1 , vector<Node> type2,vector<Node> type3, Graph<Node> &graph,Node &origin){
    graph.dijkstraShortestPath(origin);
    double minDist = INF;
    bool updated = false;
    bool updated2 = false;
    for(auto &x: type1){
        auto vert = graph.findVertex(x);
        if(vert->getDist() < minDist){
            minDist = vert->getDist();
        }
    }
    for(auto &x: type2){
        auto vert = graph.findVertex(x);
        if(vert->getDist() < minDist){
            updated = true;
            minDist = vert->getDist();
        }
    }
    for(auto &x: type3){
        auto vert = graph.findVertex(x);
        if(vert->getDist() < minDist){
            updated2 = true;
            minDist = vert->getDist();
        }
    }
    if(updated2)
        return 2;
    else if (updated)
        return 1;
    else return 0;
}

bool checkValid(int nodeId,vector<Node> strongNodes){
    for (int i = 0;i<strongNodes.size();i++){
        if(strongNodes[i] == nodeId)
            return true;
    }
    return false;
}


int main() {


    Graph<Node> graph;
    GraphViewer gv;

    LoadFiles f;

    f.loadVertexes(graph, gv); //loads vertexes

    f.loadEdges(graph, gv); //loads edges
    f.loadTags(graph, gv); //loads parks and intermediate points of interest
    f.loadStrong(graph,gv);

    cout << "\nGraph successfully loaded!\n";

    int originId;
    bool validOrigin = false;
    do{
        cout << "\nInsert the Node id of the vertex you want to start on (0-" << graph.getNumVertex() << ").\n";
        cin >> originId;
        if(!checkValid(originId,f.getStrong()) || !cin){
            cin.clear(); // reset failbit
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "\nInvalid node!\n" << endl;
        }
        else
            validOrigin = true;
    }while(!validOrigin);

    Node origin(originId); //user starting point
    gv.getNode(originId).setLabel("Origin"); //labelling origin
    gv.getNode(originId).setColor(GraphViewer::LIGHT_GRAY);

    auto startingPoint = graph.findVertex(origin);
    gv.setCenter(sf::Vector2f(startingPoint->getInfo().getxPos(), -startingPoint->getInfo().getyPos())); //sets center on user's starting point


    int destId;
    bool validDest=false;
    do{
        cout << "\nInsert the Node id of the vertex you want to finish on (0-" << graph.getNumVertex() << ").\n";
        cin >> destId;
        if(!checkValid(destId,f.getStrong()) || !cin){
            cin.clear(); // reset failbit
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "\nInvalid node!" << endl;
        }

        else if(destId == originId){
            cin.clear(); // reset failbit
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "\nInvalid node because its the same as the origin!" << endl;
        }

        else
            validDest = true;
    }while(!validDest);

    gv.getNode(destId).setLabel("Destination");
    gv.getNode(destId).setColor(GraphViewer::LIGHT_GRAY);
    Node dest(destId); //user destination


    //se 1: precorrer lista de parking para encontrar o parque mais proximo
    //se 2 percorrer lista de parking para ver qual o mais barato
    //se 3: percorrer lista de parking e procura o mais proximo do destino
    //NOTA: ainda nao estamos a ter em conta destino final em lado nenhum.. so no ponto 3??

   int choice;
   bool validChoice = false;
   do{
       cout<<"\nWhich option do you prefer to choose your park?\n";
       cout<<"\n0)Exit\n1)Closest park to your current location\n2)Cheapest park\n3)Closest park to your final destination\n";
       cin>>choice;
       if(choice < 0 || choice > 3 || !cin){
           cin.clear(); // reset failbit
           cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
           cout << "\nInvalid park option!\n";
       }
       else validChoice = true;
   }while(!validChoice);

    if(choice == 0)
       return -1;

    int algChoice;
    bool validAlgChoice=false;
    do{
        cout<<"\nDo you want to try Bellman ford(0) algorithm (not recommended) or go with Dijkstra(1)?\n";
        cin>>algChoice;
        if(algChoice > 1 || algChoice < 0 || !cin){
            cin.clear(); // reset failbit
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "\nInvalid algorithm option!\n";
        }
        else validAlgChoice = true;
    }while(!validAlgChoice);

    int option = 0;
    bool validOption = false;
    do{
        cout << "\nDo you want to pass by any pharmacy(1), gas station(3) or coffee shop(5)?(add the numbers if more than one option)\n";
        cin >> option;
        if(!cin){
            cout << "\nInvalid option!\n";
            cin.clear(); // reset failbit
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if(option == 0 || option == 1 || option == 3  || option == 5  || option == 4  || option == 6  || option == 8 || option == 9)
            validOption = true;
        else{
            cout << "\nInvalid option!\n";
            cin.clear(); // reset failbit
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }while(!validOption);


   switch (choice) {
       case 1: { //Closest park
           Node pharmacy,gas,coffee;
           Node finalPark;
           if(option == 0){ //no intermediate points

               if(algChoice)
                   graph.dijkstraShortestPath(origin);
               else graph.bellmanFordShortestPath(origin);

               pair<int, int> result = getClosestPark(f.getParking(), graph); //finds closest park
               cout << "Park n:" << result.first << " is the closest to the starting point at distance of " << result.second << "m." << endl;

               auto path = graph.getPath(origin, Node(result.first)); //finds path
               setPathColor(path, gv, 0); //draws path
               finalPark = result.first;
           }
           else if(option == 1){ //go to the pharmacy
               pharmacy = bestPharmacyOption(f.getFarmacias(), origin,  graph, gv); //find closest pharmacy

               graph.dijkstraShortestPath(origin);
               auto pathOrigin = graph.getPath(origin,pharmacy); //path from origin to pharmacy
               setPathColor(pathOrigin,gv,0);

               graph.dijkstraShortestPath(pharmacy);

               pair<int, int> result = getClosestPark(f.getParking(), graph); //closest park to the pharmacy
               cout << "Park n:" << result.first << " is the closest to the pharmacy at distance of " << result.second << "m." << endl;

               auto path = graph.getPath(pharmacy, Node(result.first));//path from pharmacy to park
               setPathColor(path, gv, 0); //draws path
               finalPark = Node(result.first);
           }
           else if(option == 3) { //go to the gas station
               gas = bestGasOption(f.getBombas(), origin, graph, gv);//find closest gas station

               graph.dijkstraShortestPath(origin);
               auto pathOrigin = graph.getPath(origin,gas); //path from origin to gas
               setPathColor(pathOrigin,gv,0); //draws path

               graph.dijkstraShortestPath(gas);
               pair<int, int> result = getClosestPark(f.getParking(), graph);
               cout << "Park n:" << result.first << " is the closest to the gas station at distance of "
                    << result.second << "m." << endl;
               auto path = graph.getPath(gas, Node(result.first));//path from gas to park
               setPathColor(path, gv, 0); //draws path
               finalPark = Node(result.first);
           }
           else if(option == 5){//go to coffee
               coffee = bestCoffeeOption(f.getCafes(), origin, graph, gv);//find closest coffee

               graph.dijkstraShortestPath(origin);
               auto pathOrigin = graph.getPath(origin,coffee);//path from origin to coffee
               setPathColor(pathOrigin,gv,0);

               graph.dijkstraShortestPath(coffee);
               pair<int, int> result = getClosestPark(f.getParking(), graph);
               cout << "Park n:" << result.first << " is the closest to the coffee at distance of "<< result.second << "m." << endl;

               auto path = graph.getPath(coffee, Node(result.first));//path from coffee to park
               setPathColor(path, gv, 0);//draws path
               finalPark = Node(result.first);
           }
           else if(option == 4){ //go to pharmacy and gas station
               int best = findClosestBusiness(f.getFarmacias(),f.getBombas(),graph,origin); //finds whether is a gas station or a pharmacy closer to the origin
               if(best == 0){ //if its a pharmacy

                   pharmacy = bestPharmacyOption(f.getFarmacias(), origin, graph, gv); //find closest pharmacy to origin
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,pharmacy);//path from origin to pharmacy
                   setPathColor(pathOrigin,gv,0);

                   gas = bestGasOption(f.getBombas(), pharmacy, graph, gv); //find closest gas station to the pharmacy
                   graph.dijkstraShortestPath(pharmacy);
                   auto pathPharmacy = graph.getPath(pharmacy,gas); //path from pharmacy to gas station
                   setPathColor(pathPharmacy,gv,0);

                   graph.dijkstraShortestPath(gas);
                   pair<int, int> result = getClosestPark(f.getParking(), graph); //closest park to the gas station
                   cout << "Park n:" << result.first << " is the closest to the gas station at distance of "<< result.second << "m." << endl;
                   auto path = graph.getPath(gas, Node(result.first)); //path from gas station to park
                   setPathColor(path, gv, 0);
                   finalPark = Node(result.first);
               }
               else{// if its a gas station
                   gas = bestGasOption(f.getBombas(), origin, graph, gv);//find closest gas station to origin
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,gas);//path from origin to gas station
                   setPathColor(pathOrigin,gv,0);

                   pharmacy = bestPharmacyOption(f.getFarmacias(), gas, graph, gv);//find closest pharmacy to gas station
                   graph.dijkstraShortestPath(gas);
                   auto pathGas = graph.getPath(gas,pharmacy);//path from gas to pharmacy
                   setPathColor(pathGas,gv,0);

                   graph.dijkstraShortestPath(pharmacy);
                   pair<int, int> result = getClosestPark(f.getParking(), graph); //closest park to the pharmacy
                   cout << "Park n:" << result.first << " is the closest to the pharmacy at distance of "<< result.second << "m." << endl;
                   auto path = graph.getPath(coffee, Node(result.first));//path from pharmacy to park
                   setPathColor(path, gv, 0);
                   finalPark = Node(result.first);
               }
           }
           else if(option == 6){ //pharmacy and coffee
               int best = findClosestBusiness(f.getFarmacias(),f.getCafes(),graph,origin);
               if(best == 0){ //pharmacy closer
                   pharmacy = bestPharmacyOption(f.getFarmacias(), origin, graph, gv); //closest pharmacy to the origin
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,pharmacy);//path from origin to pharmacy
                   setPathColor(pathOrigin,gv,0);

                   coffee = bestCoffeeOption(f.getCafes(), pharmacy, graph, gv);//closest coffee to the pharmacy
                   graph.dijkstraShortestPath(pharmacy);
                   auto pathPharmacy = graph.getPath(pharmacy,coffee);//path from pharmacy to coffee
                   setPathColor(pathPharmacy,gv,0);

                   graph.dijkstraShortestPath(coffee);
                   pair<int, int> result = getClosestPark(f.getParking(), graph);//closest park to coffee
                   cout << "Park n:" << result.first << " is the closest to the coffee at distance of "<< result.second << "m." << endl;
                   auto path = graph.getPath(coffee, Node(result.first));//path from coffee to park
                   setPathColor(path, gv, 0);
                   finalPark = Node(result.first);
               }
               else{//coffee closer
                   coffee = bestCoffeeOption(f.getCafes(), origin, graph, gv);//closest coffee to origin
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,coffee);//path from origin to coffee
                   setPathColor(pathOrigin,gv,0);

                   pharmacy = bestPharmacyOption(f.getFarmacias(), coffee, graph, gv);//pharmacy closest to coffee
                   graph.dijkstraShortestPath(coffee);
                   auto pathCoffee = graph.getPath(coffee,pharmacy);//path from coffee to pharmacy
                   setPathColor(pathCoffee,gv,0);

                   graph.dijkstraShortestPath(pharmacy);
                   pair<int, int> result = getClosestPark(f.getParking(), graph);//closest park to the pharmacy
                   cout << "Park n:" << result.first << " is the closest to the pharmacy at distance of "<< result.second << "m." << endl;
                   auto path = graph.getPath(pharmacy, Node(result.first)); //path from pharmacy to park
                   setPathColor(path, gv, 0);
                   finalPark = Node(result.first);
               }

           }
           else if(option == 8){ //gas station and coffee
               int best = findClosestBusiness(f.getBombas(),f.getCafes(),graph,origin);
               if(best == 0){ //gas station closer
                   gas = bestGasOption(f.getBombas(), origin, graph, gv); //closest gas to the origin
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,gas);//path origin to gas
                   setPathColor(pathOrigin,gv,0);

                   coffee = bestCoffeeOption(f.getCafes(), gas, graph, gv);//closest coffee to gas
                   graph.dijkstraShortestPath(gas);
                   auto pathGas = graph.getPath(gas,coffee);//path from gas to coffee
                   setPathColor(pathGas,gv,0);

                   graph.dijkstraShortestPath(coffee);
                   pair<int, int> result = getClosestPark(f.getParking(), graph); //closest park to coffee
                   cout << "Park n:" << result.first << " is the closest to the coffee at distance of "<< result.second << "m." << endl;
                   auto path = graph.getPath(coffee, Node(result.first)); //path from coffee to park
                   setPathColor(path, gv, 0);
                   finalPark = Node(result.first);
               }
               else{//coffee closer
                   coffee = bestCoffeeOption(f.getCafes(), origin, graph, gv);//closest coffee to origin
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,coffee); //path origin to coffee
                   setPathColor(pathOrigin,gv,0);

                   gas = bestGasOption(f.getBombas(), coffee, graph, gv); //closest gas to coffee
                   graph.dijkstraShortestPath(coffee);
                   auto pathGas = graph.getPath(coffee,gas); //path coffee to gas
                   setPathColor(pathGas,gv,0);

                   graph.dijkstraShortestPath(gas);
                   pair<int, int> result = getClosestPark(f.getParking(), graph); //closest park to gas
                   cout << "Park n:" << result.first << " is the closest to the gas station at distance of "<< result.second << "m." << endl;
                   auto path = graph.getPath(gas, Node(result.first));//path from gas station to park
                   setPathColor(path, gv, 0);
                   finalPark = Node(result.first);
               }
           }
           else if(option == 9){//visit all businesses
               int best = findClosestBusiness(f.getBombas(),f.getFarmacias(),f.getCafes(),graph,origin);
               if(best == 0){ //gas closer
                   gas = bestGasOption(f.getBombas(), origin, graph, gv); //closest gas to origin
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,gas); //path from origin to gas
                   setPathColor(pathOrigin,gv,0);

                   int secondBest = findClosestBusiness(f.getFarmacias(),f.getCafes(),graph,gas);

                   if(secondBest == 0){ //pharmacy 2 closest
                       pharmacy = bestPharmacyOption(f.getFarmacias(), gas, graph, gv); //pharmacy closest to the gas station
                       graph.dijkstraShortestPath(gas);
                       auto pathGas = graph.getPath(gas,pharmacy); //path from gas to pharmacy
                       setPathColor(pathGas,gv,0);

                       coffee = bestCoffeeOption(f.getCafes(), pharmacy, graph, gv);//coffee closest to the pharmacy
                       graph.dijkstraShortestPath(pharmacy);
                       auto pathPharmacy = graph.getPath(pharmacy,coffee); //path from pharmacy to coffee
                       setPathColor(pathPharmacy,gv,0);

                       graph.dijkstraShortestPath(coffee);
                       pair<int, int> result = getClosestPark(f.getParking(), graph); //closest park to the coffee
                       cout << "Park n:" << result.first << " is the closest to the coffee at distance of "<< result.second << "m." << endl;
                       auto path = graph.getPath(coffee, Node(result.first)); //path from coffee to park
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }
                   else{ //coffee 2 closest
                       coffee = bestCoffeeOption(f.getCafes(), gas, graph, gv); //coffee closest to the gas station
                       graph.dijkstraShortestPath(gas);
                       auto pathGas = graph.getPath(gas,coffee);//path from gas station to coffee
                       setPathColor(pathGas,gv,0);

                       pharmacy = bestPharmacyOption(f.getFarmacias(), coffee, graph, gv); //pharmacy closest to coffee
                       graph.dijkstraShortestPath(coffee);
                       auto pathCoffee = graph.getPath(coffee,pharmacy); //path from coffee to pharmacy
                       setPathColor(pathCoffee,gv,0);

                       graph.dijkstraShortestPath(pharmacy);
                       pair<int, int> result = getClosestPark(f.getParking(), graph); //closest park to the pharmacy
                       cout << "Park n:" << result.first << " is the closest to the pharmacy at distance of "<< result.second << "m." << endl;
                       auto path = graph.getPath(pharmacy, Node(result.first));//path from pharmacy to park
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }

               }
               else if(best == 1){ //pharmacy closest to origin
                   pharmacy = bestPharmacyOption(f.getFarmacias(), origin, graph, gv); // find closest pharmacy
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,pharmacy); //path from origin to pharmacy
                   setPathColor(pathOrigin,gv,0);

                   int secondBest = findClosestBusiness(f.getBombas(),f.getCafes(),graph,pharmacy);
                   if(secondBest == 0){ //gas station 2 closest
                       gas = bestGasOption(f.getBombas(), pharmacy, graph, gv); //closest gas station to the pharmacy
                       graph.dijkstraShortestPath(pharmacy);
                       auto pathPharmacy = graph.getPath(pharmacy,gas); //path from pharmacy to gas statiom
                       setPathColor(pathPharmacy,gv,0);

                       coffee = bestCoffeeOption(f.getCafes(), gas, graph, gv);//closest coffee to gas station
                       graph.dijkstraShortestPath(gas);
                       auto pathGas = graph.getPath(gas,coffee); //path gas station to coffee
                       setPathColor(pathGas,gv,0);

                       graph.dijkstraShortestPath(coffee);
                       pair<int, int> result = getClosestPark(f.getParking(), graph); //closest park to the coffee
                       cout << "Park n:" << result.first << " is the closest to the coffee at distance of "<< result.second << "m." << endl;
                       auto path = graph.getPath(coffee, Node(result.first)); //path from coffee to park
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }
                   else{//coffee 2 closest
                       coffee = bestCoffeeOption(f.getCafes(), pharmacy, graph, gv); //closest coffee to the pharmacy
                       graph.dijkstraShortestPath(pharmacy);
                       auto pathPharmacy = graph.getPath(pharmacy,coffee);//path pharmacy to coffee
                       setPathColor(pathPharmacy,gv,0);

                       gas = bestGasOption(f.getBombas(), coffee, graph, gv);//closest gas station to coffee
                       graph.dijkstraShortestPath(coffee);
                       auto pathGas = graph.getPath(coffee,gas); //path from coffee to gs station
                       setPathColor(pathGas,gv,0);

                       graph.dijkstraShortestPath(gas);
                       pair<int, int> result = getClosestPark(f.getParking(), graph); //closest park to the gas station
                       cout << "Park n:" << result.first << " is the closest to the gas station at distance of "<< result.second << "m." << endl;
                       auto path = graph.getPath(gas, Node(result.first)); //path from gas station to park
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }

               }
               else{ //coffee closest
                   coffee = bestCoffeeOption(f.getCafes(), origin, graph, gv); //get closest coffee to origin
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,coffee);//path from origin to coffee
                   setPathColor(pathOrigin,gv,0);

                   int secondBest = findClosestBusiness(f.getBombas(),f.getFarmacias(),graph,coffee);

                   if(secondBest == 0){//gas station 2 best
                       gas = bestGasOption(f.getBombas(), coffee, graph, gv); //closest gas to the coffee
                       graph.dijkstraShortestPath(coffee);
                       auto pathCoffee = graph.getPath(coffee,gas); //path from coffee to gas
                       setPathColor(pathCoffee,gv,0);

                       pharmacy = bestPharmacyOption(f.getFarmacias(), gas, graph, gv); //closest pharmacy to the gas station
                       graph.dijkstraShortestPath(gas);
                       auto pathGas = graph.getPath(gas,pharmacy);//path from gas to pharmacy
                       setPathColor(pathGas,gv,0);

                       graph.dijkstraShortestPath(pharmacy);
                       pair<int, int> result = getClosestPark(f.getParking(), graph); //closest park to pharmacy
                       cout << "Park n:" << result.first << " is the closest to the pharmacy at distance of "<< result.second << "m." << endl;
                       auto path = graph.getPath(pharmacy, Node(result.first)); //path from pharmacy to park
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }
                   else{//pharmacy 2 best
                       pharmacy = bestPharmacyOption(f.getFarmacias(), coffee, graph, gv);//closest pharmacy to coffee
                       graph.dijkstraShortestPath(coffee);
                       auto pathCoffee = graph.getPath(coffee,pharmacy); //path from coffee to pharmacy
                       setPathColor(pathCoffee,gv,0);

                       gas = bestGasOption(f.getBombas(), pharmacy, graph, gv);//closest gas station to the pharmacy
                       graph.dijkstraShortestPath(pharmacy);
                       auto pathPharmacy = graph.getPath(pharmacy,gas);//path pharmacy to gas station
                       setPathColor(pathPharmacy,gv,0);

                       graph.dijkstraShortestPath(gas);
                       pair<int, int> result = getClosestPark(f.getParking(), graph);//closest park to the gas station
                       cout << "Park n:" << result.first << " is the closest to the gas station at distance of "<< result.second << "m." << endl;
                       auto path = graph.getPath(gas, Node(result.first)); //path gas station to park
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }

               }
           }

           graph.dijkstraShortestPath(finalPark);
           auto pathToDest = graph.getPath(finalPark, dest); //path from park to the user's destination
           setPathColor(pathToDest, gv, 1);

           break;
       }
       case 2: {
           Node finalPark;
           Node pharmacy, coffee, gas;

           if(option == 1){
               pharmacy = bestPharmacyOption(f.getFarmacias(), origin,  graph, gv);
               graph.dijkstraShortestPath(origin);
               auto pathOrigin = graph.getPath(origin,pharmacy);
               setPathColor(pathOrigin,gv,0);

               graph.dijkstraShortestPath(pharmacy);
               pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
               cout << "Park n:" << result.first << " is the cheapest, at a distance to the pharmacy of " << result.second << "m." << endl;
               auto path = graph.getPath(pharmacy, Node(result.first));
               setPathColor(path, gv, 0);
               finalPark = Node(result.first);
           }
           else if(option == 0){
               if(algChoice)
                   graph.dijkstraShortestPath(origin);
               else graph.bellmanFordShortestPath(origin);
               pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
               cout << "Park n:" << result.first << " is the chepeast at distance of " << result.second << "m." << endl;
               auto path = graph.getPath(origin, Node(result.first));
               setPathColor(path, gv, 0);
               finalPark = result.first;
           }
           else if(option == 3) {
               gas = bestGasOption(f.getBombas(), origin, graph, gv);
               graph.dijkstraShortestPath(origin);
               auto pathOrigin = graph.getPath(origin,gas);
               setPathColor(pathOrigin,gv,0);

               graph.dijkstraShortestPath(gas);
               pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
               cout << "Park n:" << result.first << " is the closest to the gas station at distance of " << result.second << "m." << endl;
               auto path = graph.getPath(gas, Node(result.first));
               setPathColor(path, gv, 0);
               finalPark = Node(result.first); }
           else if(option == 4){
               int best = findClosestBusiness(f.getFarmacias(),f.getBombas(),graph,origin);
               if(best == 0){

                   pharmacy = bestPharmacyOption(f.getFarmacias(), origin, graph, gv);
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,pharmacy);
                   setPathColor(pathOrigin,gv,0);

                   gas = bestGasOption(f.getBombas(), pharmacy, graph, gv);
                   graph.dijkstraShortestPath(pharmacy);
                   auto pathPharmacy = graph.getPath(pharmacy,gas);
                   setPathColor(pathPharmacy,gv,0);

                   graph.dijkstraShortestPath(gas);
                   pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
                   cout << "Park n:" << result.first << " is the closest to the gas station at distance of "<< result.second << "m." << endl;
                   auto path = graph.getPath(gas, Node(result.first));
                   setPathColor(path, gv, 0);
                   finalPark = Node(result.first);
               }
               else{
                   gas = bestGasOption(f.getBombas(), origin, graph, gv);
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,gas);
                   setPathColor(pathOrigin,gv,0);

                   pharmacy = bestPharmacyOption(f.getFarmacias(), gas, graph, gv);
                   graph.dijkstraShortestPath(gas);
                   auto pathGas = graph.getPath(gas,pharmacy);
                   setPathColor(pathGas,gv,0);

                   graph.dijkstraShortestPath(pharmacy);
                   pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
                   cout << "Park n:" << result.first << " is the closest to the pharmacy at distance of "<< result.second << "m." << endl;
                   auto path = graph.getPath(coffee, Node(result.first));
                   setPathColor(path, gv, 0);
                   finalPark = Node(result.first);
               }
           }
           else if(option == 5){
               coffee = bestCoffeeOption(f.getCafes(), origin, graph, gv);
               graph.dijkstraShortestPath(origin);
               auto pathOrigin = graph.getPath(origin,coffee);
               setPathColor(pathOrigin,gv,0);

               graph.dijkstraShortestPath(coffee);
               pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
               cout << "Park n:" << result.first << " is the closest to the coffee at distance of "<< result.second << "m." << endl;
               auto path = graph.getPath(coffee, Node(result.first));
               setPathColor(path, gv, 0);
               finalPark = Node(result.first);
           }
           else if(option == 6){
               int best = findClosestBusiness(f.getFarmacias(),f.getCafes(),graph,origin);
               if(best == 0){
                   pharmacy = bestPharmacyOption(f.getFarmacias(), origin, graph, gv);
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,pharmacy);
                   setPathColor(pathOrigin,gv,0);

                   coffee = bestCoffeeOption(f.getCafes(), pharmacy, graph, gv);
                   graph.dijkstraShortestPath(pharmacy);
                   auto pathPharmacy = graph.getPath(pharmacy,coffee);
                   setPathColor(pathPharmacy,gv,0);

                   graph.dijkstraShortestPath(coffee);
                   pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
                   cout << "Park n:" << result.first << " is the closest to the coffee at distance of "<< result.second << "m." << endl;
                   auto path = graph.getPath(coffee, Node(result.first));
                   setPathColor(path, gv, 0);
                   finalPark = Node(result.first);
               }
               else{
                   coffee = bestCoffeeOption(f.getCafes(), origin, graph, gv);
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,coffee);
                   setPathColor(pathOrigin,gv,0);

                   pharmacy = bestPharmacyOption(f.getFarmacias(), coffee, graph, gv);
                   graph.dijkstraShortestPath(coffee);
                   auto pathCoffee = graph.getPath(coffee,pharmacy);
                   setPathColor(pathCoffee,gv,0);

                   graph.dijkstraShortestPath(pharmacy);
                   pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
                   cout << "Park n:" << result.first << " is the closest to the pharmacy at distance of "<< result.second << "m." << endl;
                   auto path = graph.getPath(pharmacy, Node(result.first));
                   setPathColor(path, gv, 0);
                   finalPark = Node(result.first);
               }

           }
           else if(option == 8){
               int best = findClosestBusiness(f.getBombas(),f.getCafes(),graph,origin);
               if(best == 0){
                   gas = bestGasOption(f.getBombas(), origin, graph, gv);
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,gas);
                   setPathColor(pathOrigin,gv,0);

                   coffee = bestCoffeeOption(f.getCafes(), gas, graph, gv);
                   graph.dijkstraShortestPath(gas);
                   auto pathGas = graph.getPath(gas,coffee);
                   setPathColor(pathGas,gv,0);

                   graph.dijkstraShortestPath(coffee);
                   pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
                   cout << "Park n:" << result.first << " is the closest to the coffee at distance of "<< result.second << "m." << endl;
                   auto path = graph.getPath(coffee, Node(result.first));
                   setPathColor(path, gv, 0);
                   finalPark = Node(result.first);
               }
               else{
                   coffee = bestCoffeeOption(f.getCafes(), origin, graph, gv);
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,coffee);
                   setPathColor(pathOrigin,gv,0);

                   gas = bestGasOption(f.getBombas(), coffee, graph, gv);
                   graph.dijkstraShortestPath(coffee);
                   auto pathGas = graph.getPath(coffee,gas);
                   setPathColor(pathGas,gv,0);

                   graph.dijkstraShortestPath(gas);
                   pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
                   cout << "Park n:" << result.first << " is the closest to the gas station at distance of "<< result.second << "m." << endl;
                   auto path = graph.getPath(gas, Node(result.first));
                   setPathColor(path, gv, 0);
                   finalPark = Node(result.first);
               }
           }
           else if(option == 9){
               int best = findClosestBusiness(f.getBombas(),f.getFarmacias(),f.getCafes(),graph,origin);
               if(best == 0){
                   gas = bestGasOption(f.getBombas(), origin, graph, gv);
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,gas);
                   setPathColor(pathOrigin,gv,0);

                   int secondBest = findClosestBusiness(f.getFarmacias(),f.getCafes(),graph,gas);

                   if(secondBest == 0){
                       pharmacy = bestPharmacyOption(f.getFarmacias(), gas, graph, gv);
                       graph.dijkstraShortestPath(gas);
                       auto pathGas = graph.getPath(gas,pharmacy);
                       setPathColor(pathGas,gv,0);

                       coffee = bestCoffeeOption(f.getCafes(), pharmacy, graph, gv);
                       graph.dijkstraShortestPath(pharmacy);
                       auto pathPharmacy = graph.getPath(pharmacy,coffee);
                       setPathColor(pathPharmacy,gv,0);

                       graph.dijkstraShortestPath(coffee);
                       pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
                       cout << "Park n:" << result.first << " is the closest to the coffe at distance of "<< result.second << "m." << endl;
                       auto path = graph.getPath(coffee, Node(result.first));
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }
                   else{
                       coffee = bestCoffeeOption(f.getCafes(), gas, graph, gv);
                       graph.dijkstraShortestPath(gas);
                       auto pathGas = graph.getPath(gas,coffee);
                       setPathColor(pathGas,gv,0);

                       pharmacy = bestPharmacyOption(f.getFarmacias(), coffee, graph, gv);
                       graph.dijkstraShortestPath(coffee);
                       auto pathCoffee = graph.getPath(coffee,pharmacy);
                       setPathColor(pathCoffee,gv,0);

                       graph.dijkstraShortestPath(pharmacy);
                       pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
                       cout << "Park n:" << result.first << " is the closest to the pharmacy at distance of "<< result.second << "m." << endl;
                       auto path = graph.getPath(pharmacy, Node(result.first));
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }

               }
               else if(best == 1){
                   pharmacy = bestPharmacyOption(f.getFarmacias(), origin, graph, gv);
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,pharmacy);
                   setPathColor(pathOrigin,gv,0);

                   int secondBest = findClosestBusiness(f.getBombas(),f.getCafes(),graph,pharmacy);
                   if(secondBest == 0){
                       gas = bestGasOption(f.getBombas(), pharmacy, graph, gv);
                       graph.dijkstraShortestPath(pharmacy);
                       auto pathPharmacy = graph.getPath(pharmacy,gas);
                       setPathColor(pathPharmacy,gv,0);

                       coffee = bestCoffeeOption(f.getCafes(), gas, graph, gv);
                       graph.dijkstraShortestPath(gas);
                       auto pathGas = graph.getPath(gas,coffee);
                       setPathColor(pathGas,gv,0);

                       graph.dijkstraShortestPath(coffee);
                       pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
                       cout << "Park n:" << result.first << " is the closest to the coffee at distance of "<< result.second << "m." << endl;
                       auto path = graph.getPath(coffee, Node(result.first));
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }
                   else{
                       coffee = bestCoffeeOption(f.getCafes(), pharmacy, graph, gv);
                       graph.dijkstraShortestPath(pharmacy);
                       auto pathPharmacy = graph.getPath(pharmacy,coffee);
                       setPathColor(pathPharmacy,gv,0);

                       gas = bestGasOption(f.getBombas(), coffee, graph, gv);
                       graph.dijkstraShortestPath(coffee);
                       auto pathGas = graph.getPath(coffee,gas);
                       setPathColor(pathGas,gv,0);

                       graph.dijkstraShortestPath(gas);
                       pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
                       cout << "Park n:" << result.first << " is the closest to the gas station at distance of "<< result.second << "m." << endl;
                       auto path = graph.getPath(gas, Node(result.first));
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }

               }
               else{
                   coffee = bestCoffeeOption(f.getCafes(), origin, graph, gv);
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,coffee);
                   setPathColor(pathOrigin,gv,0);

                   int secondBest = findClosestBusiness(f.getBombas(),f.getFarmacias(),graph,coffee);

                   if(secondBest == 0){
                       gas = bestGasOption(f.getBombas(), coffee, graph, gv);
                       graph.dijkstraShortestPath(coffee);
                       auto pathCoffee = graph.getPath(coffee,gas);
                       setPathColor(pathCoffee,gv,0);

                       pharmacy = bestPharmacyOption(f.getFarmacias(), gas, graph, gv);
                       graph.dijkstraShortestPath(gas);
                       auto pathGas = graph.getPath(gas,pharmacy);
                       setPathColor(pathGas,gv,0);

                       graph.dijkstraShortestPath(pharmacy);
                       pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
                       cout << "Park n:" << result.first << " is the closest to the pharmacy at distance of "<< result.second << "m." << endl;
                       auto path = graph.getPath(pharmacy, Node(result.first));
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }
                   else{
                       pharmacy = bestPharmacyOption(f.getFarmacias(), coffee, graph, gv);
                       graph.dijkstraShortestPath(coffee);
                       auto pathCoffee = graph.getPath(coffee,pharmacy);
                       setPathColor(pathCoffee,gv,0);

                       gas = bestGasOption(f.getBombas(), pharmacy, graph, gv);
                       graph.dijkstraShortestPath(pharmacy);
                       auto pathPharmacy = graph.getPath(pharmacy,gas);
                       setPathColor(pathPharmacy,gv,0);

                       graph.dijkstraShortestPath(gas);
                       pair<int, int> result = getCheapestPark(f.getParking(), graph,f.getPrices());
                       cout << "Park n:" << result.first << " is the closest to the gas station at distance of "<< result.second << "m." << endl;
                       auto path = graph.getPath(gas, Node(result.first));
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }

               }
           }

           graph.dijkstraShortestPath(finalPark);
           auto pathToDest = graph.getPath(finalPark, dest);
           setPathColor(pathToDest, gv, 1);

           break;
       }
       case 3: {
           pair<int, int> result = getDestinationPark(f.getParking(), dest, graph);
           Node finalPark(result.first);
           Node pharmacy, coffee, gas;
           if(option == 0) {
               graph.dijkstraShortestPath(origin);
               auto path = graph.getPath(origin, finalPark);
               setPathColor(path, gv, 0);
           }
           else if(option == 1){
               pharmacy = bestPharmacyOption(f.getFarmacias(), finalPark,  graph, gv);
               graph.dijkstraShortestPath(origin);
               auto pathPharmacy = graph.getPath(origin,pharmacy);
               setPathColor(pathPharmacy, gv, 0);

               graph.dijkstraShortestPath(pharmacy);
               auto path = graph.getPath(pharmacy, finalPark);
               setPathColor(path, gv, 0);

           }
           else if(option == 3){
               gas = bestGasOption(f.getBombas(), finalPark, graph, gv);
               graph.dijkstraShortestPath(origin);
               auto pathGas = graph.getPath(origin,gas);
               setPathColor(pathGas, gv, 0);

               graph.dijkstraShortestPath(gas);
               auto path = graph.getPath(gas, finalPark);
               setPathColor(path, gv, 0);
           }
           else if(option == 4){
               int best = findClosestBusiness(f.getFarmacias(),f.getBombas(),graph,finalPark);
               if(best == 0){
                   gas = bestGasOption(f.getBombas(), origin, graph, gv);
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,gas);
                   setPathColor(pathOrigin,gv,0);

                   pharmacy = bestPharmacyOption(f.getFarmacias(), gas, graph, gv);
                   graph.dijkstraShortestPath(gas);
                   auto pathGas = graph.getPath(gas,pharmacy);
                   setPathColor(pathGas,gv,0);

                   graph.dijkstraShortestPath(pharmacy);
                   auto path = graph.getPath(pharmacy, finalPark);
                   setPathColor(path, gv, 0);
               }
               else{
                   pharmacy = bestPharmacyOption(f.getFarmacias(), origin, graph, gv);
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,pharmacy);
                   setPathColor(pathOrigin,gv,0);

                   gas = bestGasOption(f.getBombas(), pharmacy, graph, gv);
                   graph.dijkstraShortestPath(pharmacy);
                   auto pathPharmacy = graph.getPath(pharmacy,gas);
                   setPathColor(pathPharmacy,gv,0);

                   graph.dijkstraShortestPath(gas);
                   auto path = graph.getPath(gas, finalPark);
                   setPathColor(path, gv, 0);
               }
           }

           else if(option == 5){
               coffee = bestCoffeeOption(f.getCafes(), finalPark, graph, gv);
               graph.dijkstraShortestPath(origin);
               auto pathCoffee= graph.getPath(origin,coffee);
               setPathColor(pathCoffee, gv, 0);

               graph.dijkstraShortestPath(coffee);
               auto path = graph.getPath(coffee, finalPark);
               setPathColor(path, gv, 0);
           }


           else if(option == 6){
               int best = findClosestBusiness(f.getFarmacias(),f.getCafes(),graph,finalPark);
               if(best == 0){
                   coffee = bestCoffeeOption(f.getCafes(), origin, graph, gv);
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,coffee);
                   setPathColor(pathOrigin,gv,0);

                   pharmacy = bestPharmacyOption(f.getFarmacias(), coffee, graph, gv);
                   graph.dijkstraShortestPath(coffee);
                   auto pathCoffee = graph.getPath(coffee,pharmacy);
                   setPathColor(pathCoffee,gv,0);

                   graph.dijkstraShortestPath(pharmacy);
                   auto path = graph.getPath(pharmacy, finalPark);
                   setPathColor(path, gv, 0);
               }
               else{
                   pharmacy = bestPharmacyOption(f.getFarmacias(), origin, graph, gv);
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,pharmacy);
                   setPathColor(pathOrigin,gv,0);

                   coffee = bestCoffeeOption(f.getCafes(), pharmacy, graph, gv);
                   graph.dijkstraShortestPath(pharmacy);
                   auto pathPharmacy = graph.getPath(pharmacy,coffee);
                   setPathColor(pathPharmacy,gv,0);

                   graph.dijkstraShortestPath(coffee);
                   auto path = graph.getPath(coffee, finalPark);
                   setPathColor(path, gv, 0);
               }
           }
          else if(option == 8){
               int best = findClosestBusiness(f.getBombas(),f.getCafes(),graph,finalPark);
               if(best == 0){
                   coffee = bestCoffeeOption(f.getCafes(), origin, graph, gv);
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,coffee);
                   setPathColor(pathOrigin,gv,0);

                   gas = bestGasOption(f.getBombas(), coffee, graph, gv);
                   graph.dijkstraShortestPath(coffee);
                   auto pathCoffee = graph.getPath(coffee,gas);
                   setPathColor(pathCoffee,gv,0);

                   graph.dijkstraShortestPath(gas);
                   auto path = graph.getPath(gas, finalPark);
                   setPathColor(path, gv, 0);
                   finalPark = Node(result.first);
               }
               else{
                   gas = bestGasOption(f.getBombas(), origin, graph, gv);
                   graph.dijkstraShortestPath(origin);
                   auto pathOrigin = graph.getPath(origin,gas);
                   setPathColor(pathOrigin,gv,0);

                   coffee = bestCoffeeOption(f.getCafes(), gas, graph, gv);
                   graph.dijkstraShortestPath(gas);
                   auto pathGas = graph.getPath(gas,coffee);
                   setPathColor(pathGas,gv,0);

                   graph.dijkstraShortestPath(coffee);
                   auto path = graph.getPath(coffee, finalPark);
                   setPathColor(path, gv, 0);
                   finalPark = Node(result.first);
               }
          }

          else if(option == 9){
               int best = findClosestBusiness(f.getBombas(),f.getFarmacias(),f.getCafes(),graph,finalPark);
               if(best == 0){

                   int secondBest = findClosestBusiness(f.getFarmacias(),f.getCafes(),graph,finalPark);

                   if(secondBest == 0){
                       coffee = bestCoffeeOption(f.getCafes(), origin, graph, gv);
                       graph.dijkstraShortestPath(origin);
                       auto pathOrigin = graph.getPath(origin,coffee);
                       setPathColor(pathOrigin,gv,0);

                       pharmacy = bestPharmacyOption(f.getFarmacias(), coffee, graph, gv);
                       graph.dijkstraShortestPath(coffee);
                       auto pathCoffee = graph.getPath(coffee,pharmacy);
                       setPathColor(pathCoffee,gv,0);

                       gas = bestGasOption(f.getBombas(), pharmacy, graph, gv);
                       graph.dijkstraShortestPath(pharmacy);
                       auto pathPharmacy = graph.getPath(pharmacy,gas);
                       setPathColor(pathPharmacy,gv,0);

                       graph.dijkstraShortestPath(gas);
                       auto path = graph.getPath(gas, finalPark);
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }
                   else{
                       pharmacy = bestPharmacyOption(f.getFarmacias(), origin, graph, gv);
                       graph.dijkstraShortestPath(origin);
                       auto pathOrigin = graph.getPath(origin,pharmacy);
                       setPathColor(pathOrigin,gv,0);

                       coffee = bestCoffeeOption(f.getCafes(), pharmacy, graph, gv);
                       graph.dijkstraShortestPath(pharmacy);
                       auto pathPharmacy = graph.getPath(pharmacy,coffee);
                       setPathColor(pathPharmacy,gv,0);

                       gas = bestGasOption(f.getBombas(), coffee, graph, gv);
                       graph.dijkstraShortestPath(coffee);
                       auto pathCoffee = graph.getPath(coffee,gas);
                       setPathColor(pathCoffee,gv,0);

                       graph.dijkstraShortestPath(gas);
                       auto path = graph.getPath(gas, finalPark);
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }

               }
               else if(best == 1){
                   int secondBest = findClosestBusiness(f.getBombas(),f.getCafes(),graph,finalPark);
                   if(secondBest == 0){
                       coffee = bestCoffeeOption(f.getCafes(), origin, graph, gv);
                       graph.dijkstraShortestPath(origin);
                       auto pathOrigin = graph.getPath(origin,coffee);
                       setPathColor(pathOrigin,gv,0);

                       gas = bestGasOption(f.getBombas(), coffee, graph, gv);
                       graph.dijkstraShortestPath(coffee);
                       auto pathCoffee = graph.getPath(coffee,gas);
                       setPathColor(pathCoffee,gv,0);

                       pharmacy = bestPharmacyOption(f.getFarmacias(), gas, graph, gv);
                       graph.dijkstraShortestPath(gas);
                       auto pathGas = graph.getPath(gas,pharmacy);
                       setPathColor(pathGas,gv,0);

                       graph.dijkstraShortestPath(pharmacy);
                       auto path = graph.getPath(pharmacy, finalPark);
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }
                   else{
                       gas = bestGasOption(f.getBombas(), origin, graph, gv);
                       graph.dijkstraShortestPath(origin);
                       auto pathOrigin = graph.getPath(origin,gas);
                       setPathColor(pathOrigin,gv,0);

                       coffee = bestCoffeeOption(f.getCafes(), gas, graph, gv);
                       graph.dijkstraShortestPath(gas);
                       auto pathGas = graph.getPath(gas,coffee);
                       setPathColor(pathGas,gv,0);

                       pharmacy = bestPharmacyOption(f.getFarmacias(), coffee, graph, gv);
                       graph.dijkstraShortestPath(coffee);
                       auto pathCoffee = graph.getPath(coffee,pharmacy);
                       setPathColor(pathCoffee,gv,0);

                       graph.dijkstraShortestPath(pharmacy);
                       auto path = graph.getPath(pharmacy,finalPark);
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }

               }
               else{
                   int secondBest = findClosestBusiness(f.getBombas(),f.getFarmacias(),graph,finalPark);

                   if(secondBest == 0){
                       pharmacy = bestPharmacyOption(f.getFarmacias(), origin, graph, gv);
                       graph.dijkstraShortestPath(origin);
                       auto pathOrigin = graph.getPath(origin,pharmacy);
                       setPathColor(pathOrigin,gv,0);

                       gas = bestGasOption(f.getBombas(), pharmacy, graph, gv);
                       graph.dijkstraShortestPath(pharmacy);
                       auto pathPharmacy = graph.getPath(pharmacy,gas);
                       setPathColor(pathPharmacy,gv,0);

                       coffee = bestCoffeeOption(f.getCafes(), gas, graph, gv);
                       graph.dijkstraShortestPath(gas);
                       auto pathGas = graph.getPath(gas,coffee);
                       setPathColor(pathGas,gv,0);

                       graph.dijkstraShortestPath(coffee);
                       auto path = graph.getPath(coffee, finalPark);
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }
                   else{
                       gas = bestGasOption(f.getBombas(), origin, graph, gv);
                       graph.dijkstraShortestPath(origin);
                       auto pathOrigin = graph.getPath(origin,gas);
                       setPathColor(pathOrigin,gv,0);

                       pharmacy = bestPharmacyOption(f.getFarmacias(), gas, graph, gv);
                       graph.dijkstraShortestPath(gas);
                       auto pathGas= graph.getPath(gas,pharmacy);
                       setPathColor(pathGas,gv,0);

                       coffee = bestCoffeeOption(f.getCafes(), pharmacy, graph, gv);
                       graph.dijkstraShortestPath(pharmacy);
                       auto pathPharmacy = graph.getPath(pharmacy,coffee);
                       setPathColor(pathPharmacy,gv,0);

                       graph.dijkstraShortestPath(coffee);
                       auto path = graph.getPath(coffee, finalPark);
                       setPathColor(path, gv, 0);
                       finalPark = Node(result.first);
                   }

               }
          }
           graph.dijkstraShortestPath(finalPark);
           auto pathToDest = graph.getPath(finalPark, dest);
           setPathColor(pathToDest, gv, 1);

           break;
       }
       case 0:
           break;
       default:
           break;
   }
   cout<< "Captions:\nParks: Blue\nPharmacies: Pink\nCoffee shops: Cyan\nGas stations: Yellow\n";
   cout<< "Path captions:\nPath to Park(Car): Green\nPath do destination(On Foot): Orange\n";
   gv.createWindow(600, 600);

   gv.join();

    return 0;
}
