//
// Created by fadib on 2021-09-25.
//

#ifndef COMP345_N11_MAP_H
#define COMP345_N11_MAP_H

#include <string>
#include "../Player/Player.h"
#include <string>
#include <regex>
#include <fstream>
//#include <sstream>
#include <iostream>


using namespace std;


class Map;
class Territory;
class Continent;

class MapLoader{
public:
    MapLoader();
    explicit MapLoader(string fileName);
    MapLoader(MapLoader const &anotherMapLoader);
    MapLoader& operator = (MapLoader const &anotherMapLoader);
    ~MapLoader();
    void loadFromFile(string fileName);
private:
    Territory** territories;
    Continent** continents;
    int numContinents, numTerritories;
    friend Map;
    friend ostream& operator << (ostream &stream, const MapLoader &mapLoader);
};

class Map{
public:
    Map();
    Map(MapLoader const &mapLoader);
    Map(Map const &anotherMap);
    Map& operator = (Map const &anotherMap);
    ~Map();
private:
    Territory** territories;
    Continent** continents;
    int numContinents, numTerritories;
    friend ostream& operator << (ostream &stream, const Map &map);
};

class Territory{
public:
    Territory();
    Territory(int id, string name, int army);
    void addAdjTerritories(Territory* adjTer);


    // Thong
    int getNumberOfArmies() const;
    int getPendingIncomingArmies() const;
    int getPendingOutgoingArmies() const;
    void setName(std::string name);
    void setPendingIncomingArmies(int armies);
    void setPendingOutgoingArmies(int armies);
    void removeArmies(int armies);
    void addArmies(int armies);
    void addPendingIncomingArmies(int armies);
    void addPendingOutgoingArmies(int armies);
    int getNumberOfMovableArmies();

    Territory(int id, string name, int armies, Continent *continent);
    Territory(Territory const &anotherTerritory);
    Territory& operator = (Territory const &anotherTerritory);
    void addAdjTerritories(int numAdjTerritories, Territory **adjTerritories);
    ~Territory();
    void setOwner(Player *owner);
    Player* getOwner();
    int getId();
    void setArmies(int newArmies);
    int getArmies();
    void removeOwner();
private:
    Territory** adjTerritories;
    Continent* continent;
    Player* owner;
    int armies;
    int id;
    int numAdjTerritories;
    string name;
    friend Continent;
    friend MapLoader;
    friend ostream& operator << (ostream &stream, const Territory &territory);

    // thong
    int numberOfArmies_;
    int pendingIncomingArmies_;
    int pendingOutgoingArmies_;
};

class Continent{
public:
    Continent();
    Continent(int id, string name, int bonus);
    Continent(Continent const &anotherContinent);
    Continent& operator = (Continent const &anotherContinent);
    string getName();
    ~Continent();
    Player* getOwner();

private:
    int id;
    string name;
    int bonus;
    Territory** territories;
    int numTerritories;
    Player* owner;
    void setTerritories(int numTerritories, Territory** territories);
    friend MapLoader;
    friend Territory;
    friend ostream& operator << (ostream& stream, const Continent &continent);
};

ostream& operator << (ostream &stream, const MapLoader &mapLoader);
ostream& operator << (ostream &stream, const Map &map);
ostream& operator << (ostream &stream, const Territory &territory);
ostream& operator << (ostream &stream, const Continent &continent);


#endif //COMP345_N11_MAP_H
