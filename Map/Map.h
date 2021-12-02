//
// Created by fadib on 2021-09-25.
//

#ifndef COMP345_N11_MAP_H
#define COMP345_N11_MAP_H

#include <string>
#include "../Player/Player.h"
#include "../GameEngine/GameEngine.h"
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>


using namespace std;


class Map;
class Territory;
class Continent;
class Player;
class GameEngine;

class MapLoader{
public:
    MapLoader();
    MapLoader(MapLoader const &anotherMapLoader);
    MapLoader& operator = (MapLoader const &anotherMapLoader);
    ~MapLoader();

    static Map* loadMapFile(string fileName);

private:
    friend Map;
    friend ostream& operator << (ostream &stream, const MapLoader &mapLoader);
};

class Map{
public:
    Map();
    Map(Map const &anotherMap);
    Map& operator = (Map const &anotherMap);
    ~Map();
    int validate() const;
    void setOwnerOfTerritory(Player *player); ///////////// for demo purpose only, to be removed later
    void makeContinentContain2Territories(); ///////////// for demo purpose only, to be removed later

    ///////////////////////////////////////////////////////////SARAH
    int getNumContinent();
    Continent ** getContinent() const;
    ///////////////////////////////////////////////////////////////

    int getNumTerritories();
    Territory **getTerritories() const;
    std::vector<Territory*> getAdjacentTerritories(Territory* territory);
private:
    Territory** territories;
    Continent** continents;
    int numContinents, numTerritories;
    bool checkConnectivity(Territory *ter, Territory **path, bool withinContinent) const;
    friend Map* MapLoader::loadMapFile(string fileName);
    friend ostream& operator << (ostream &stream, const Map &map);
    std::unordered_map<Territory*, std::vector<Territory*>> adjacencyList_;
};

class Territory{
public:
    Territory();
//    Territory(string name);
//    Territory(int id, string name, int army);
//    void addAdjTerritories(Territory* adjTer);
    Territory(int id, string name, int armies, Continent *continent);
    Territory(Territory const &anotherTerritory);
    Territory& operator = (Territory const &anotherTerritory);
    void setAdjTerritories(int numAdjTerritories, Territory **adjTerritories);
    ~Territory();
    void setOwner(Player *owner);
    Player* getOwner() const;
    int getId() const;
    string getName() const;
    void setNumberOfArmies(int newArmies);
    void removeOwner();
    int getNumberOfArmies() const;
    int getPendingIncomingArmies() const;
    int getPendingOutgoingArmies() const;
    void setPendingIncomingArmies(int armies);
    void setPendingOutgoingArmies(int armies);
    void removeArmies(int armies);
    void addArmies(int armies);
    void addPendingIncomingArmies(int armies);
    void addPendingOutgoingArmies(int armies);
    int getNumberOfMovableArmies() const;
    Continent * getContinent() const;

    /////////////////////////////////////////////////Sarah
    int getNumAdjTerritories() const;
    Territory ** getAdjTerritories() const;
    ///////////////////////////////////////////////
private:
    Territory** adjTerritories;
    Continent* continent;
    Player* owner;
    int numArmies;
    int id;
    int numAdjTerritories;
    string name;
    friend Continent;
    friend MapLoader;
    friend Map;
    friend ostream& operator << (ostream &stream, const Territory &territory);
    int pendingIncomingArmies;
    int pendingOutgoingArmies;

};

class Continent{
public:
    Continent();
    Continent(int id, string name, int bonus);
    Continent(Continent const &anotherContinent);
    Continent& operator = (Continent const &anotherContinent);
    string getName() const;
    ~Continent();
    Player* getOwner() const;
    ////////////////////////////////////////////////////////// SARAH
    int getBonus() const;
    /////////////////////////////////////////////////////////////////
private:
    int id;
    string name;
    int bonus;
    Territory** territories;
    int numTerritories;
    Player* owner;
    void setTerritories(int numTerritories, Territory** territories);
    friend MapLoader;
    friend Map;
    friend Territory;
    friend ostream& operator << (ostream& stream, const Continent &continent);
};

ostream& operator << (ostream &stream, const MapLoader &mapLoader);
ostream& operator << (ostream &stream, const Map &map);
ostream& operator << (ostream &stream, const Territory &territory);
ostream& operator << (ostream &stream, const Continent &continent);


#endif //COMP345_N11_MAP_H
