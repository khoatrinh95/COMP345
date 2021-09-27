//
// Created by fadib on 2021-09-25.
//

#ifndef COMP345_N11_MAP_H
#define COMP345_N11_MAP_H

#include <string>
using namespace std;

class Map;
class Territory;
class Continent;

class MapLoader{
public:
    MapLoader();
    explicit MapLoader(string fileName);
    void loadFromFile(string fileName);
private:
    Map* map;
};

class Map{
public:
    Map();
private:
    Territory* territory;

};

class Territory{
public:
    Territory();
    Territory(int id, string name, int army);
    void addAdjTerritories(Territory* adjTer);
private:
    Territory* adjTerritory;
    Continent* continent;
//    Player* player;
    int id, army;
    string name;
};

class Continent{
public:
    Continent();
private:
    Territory* territory;
    int id;
    string name;
};

#endif //COMP345_N11_MAP_H
