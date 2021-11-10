//
// Created by Fadi Albasha on 2021-09-25.
//

#include "../Map/Map.h"

// MapLoader default constructor
MapLoader::MapLoader() {
    // nothing
}

// MapLoader copy constructor
MapLoader::MapLoader(MapLoader const &anotherMapLoader) {
    // nothing
}

// overloading the assignment operator for MapLoader
MapLoader& MapLoader::operator = (MapLoader const &anotherMapLoader) {
    // nothing
    return *this;
}

// MapLoader destructor
MapLoader::~MapLoader() {
    // nothing
}

// static function that reads a map file and returns a pointer to a map object
Map* MapLoader::loadMapFile(std::string fileName) {
    ifstream mapFile(fileName);
    Map* map = new Map();
    string line;
    stringstream strStream;
    regex regexSection("(\\[)(.+)(\\])");
    smatch strMatch;
    map->numContinents = 0;
    map->numTerritories = 0;
    int indexContinents = 0;
    int indexCountries = 0;
    int* borders = nullptr;
    int* numCntryInContnt;

    // looping on map file twice:
    // iter. no.1 - to get number of continents, territories ...
    // iter. no.2 - to instantiate objects of continents, territories ...
    for(int i = 0; i < 2; i++) {
        bool readFlag(false);
        bool regexFlag(false);
        string section;
        string prevSection;
        int prevContntId = 0;
        int cntryInContnt = 0;
        if (mapFile.eof()) {
            mapFile.clear();
            mapFile.seekg(0, ifstream::beg);
        }
        while (!mapFile.eof() && !mapFile.fail()) {
            getline(mapFile, line);

            // get a section's name
            regexFlag = regex_match(line, strMatch, regexSection);
            if (regexFlag) {
                prevSection = section;
                section = strMatch[2].str();
            }

            // After end of each section in the first iteration
            if (i==0 && line.length() == 0 && !section.empty()) {
                // creating the map's list of continents
                if (section == "continents") {
                    if (map->numContinents > 0) {
                        map->continents = new Continent*[map->numContinents];
                        numCntryInContnt = new int[map->numContinents];
                        for(int j = 0; j < map->numContinents; j++) {
                            numCntryInContnt[j] = 0;
                        }
                    }
                }

                // creating the map's list of territories
                else if (section == "countries") {
                    if (map->numTerritories > 0) {
                        map->territories = new Territory*[map->numTerritories];
                        borders = new int[map->numTerritories];
                        for (int j = 0; j < map->numTerritories; j++) {
                            borders[j] = 0;
                        }
                    }
                } else if (section == "borders") {
                    // do nothing
                }
                // reset section
                section = "";
            }

            // for lines in a section
            if (!regexFlag && line.length() > 0 && !section.empty()) {
                // reset StringStream object
                strStream.str(line);
                strStream.clear();

                // if iter 1: count number of continents
                // if iter 2: create Continent object when map-file's line is proper
                if(section == "continents") {
                    if(i==0) {
                        map->numContinents++;
                    } else {
                        string name;
                        int bonus;
                        string color;
                        if (strStream >> name >> bonus >> color) {
                            map->continents[indexContinents] = new Continent(indexContinents+1, name, bonus);
                        } else {
                            map->continents[indexContinents] = nullptr;
                        }
                        indexContinents++;
                    }
                }

                // if iter 1: count number of territories
                // if iter 2: create Territory object when map-file's line is proper, add territories to their continents
                else if(section == "countries") {
                    if(i==0) {
                        map->numTerritories++;
                    } else {
                        int id;
                        string name;
                        int continentId;
                        int x, y;
                        if (strStream >> id >> name >> continentId >> x >> y) {
                            if (indexCountries == id - 1) {
                                map->territories[indexCountries] = new Territory(id, name, 0, map->continents[continentId - 1]);
                            } else {
                                map->territories[indexCountries] = nullptr;
                            }
                        } else {
                            map->territories[indexCountries] = nullptr;
                        }
                        indexCountries++;
                        if (prevContntId == continentId) {
                            cntryInContnt++;
                        } else if (prevContntId != continentId) {
                            if (prevContntId > 0) {
                                numCntryInContnt[prevContntId-1] = cntryInContnt;
                                cntryInContnt = 1;
                            }
                            prevContntId = continentId;
                        }

                        // adding territories to their continents
                        if (indexCountries == map->numTerritories && continentId > 0) {
                            numCntryInContnt[continentId-1] = cntryInContnt;
                        }
                        if (indexCountries == map->numTerritories) {
                            for(int j = 0; j < map->numContinents; j++) {
                                Territory** ter = new Territory*[numCntryInContnt[j]];
                                for (int k = 0; k < numCntryInContnt[j]; k++) {
                                    ter[k] = nullptr;
                                }
                                int index = 0;
                                for (int k = 0; k < map->numTerritories; k++) {
                                    if (map->territories[k]->continent->id == (j+1)){
                                        ter[index++] = map->territories[k];
                                    }
                                }
                                map->continents[j]->territories = ter;
                                map->continents[j]->numTerritories = numCntryInContnt[j];
                            }
                        }
                    }
                }

                // if iter 1: count number of borders
                // if iter 2: create and populate list of adjacent territories for each territory when map-file's line is proper
                else if(section == "borders") {
                    int numBorders = 0;
                    int adjTerId = -1;
                    int terId = -1;
                    strStream >> terId;
                    if (terId > 0) {
                        if (i == 0) {
                            while(strStream >> adjTerId) {
                                borders[terId - 1]++;
                            }
                        } else {
                            Territory** adjTer = new Territory*[borders[terId-1]];
                            for (int j = 0; j < borders[terId-1]; j++) {
                                adjTer[j] = nullptr;
                            }
                            int index = 0;
                            while(strStream >> adjTerId) {
                                if (adjTerId > 0) {
                                    adjTer[index++] = *(map->territories + adjTerId - 1);
                                }
                            }
                            map->territories[terId-1]->adjTerritories = adjTer;
                            map->territories[terId-1]->numAdjTerritories = borders[terId-1];
                        }
                    }
                }
            }
        }
    }

    // deallocating memory
    delete[] borders;
    borders = nullptr;
    delete[] numCntryInContnt;
    numCntryInContnt = nullptr;

    // closing map file
    mapFile.close();

    return map;
}

// Map default constructor
Map::Map() {
    territories = nullptr;
    continents = nullptr;
    numTerritories = 0;
    numContinents = 0;
}

// Map copy constructor
Map::Map(Map const &anotherMap) {
    territories = nullptr;
    continents = nullptr;
    numTerritories = anotherMap.numTerritories;
    numContinents = anotherMap.numContinents;

    // copying list of territories
    if (anotherMap.territories != nullptr && anotherMap.numTerritories > 0) {
        territories = new Territory*[numTerritories];
        for (int i = 0; i < numTerritories; i++) {
            territories[i] = new Territory(*(anotherMap.territories[i]));
        }
    }

    // copying list of continents
    if (anotherMap.continents != nullptr && anotherMap.numContinents > 0) {
        continents = new Continent*[numContinents];
        for (int i = 0; i < numContinents; i++) {
            continents[i] = new Continent(*(anotherMap.continents[i]));
        }
    }
    // linking adjacent territories, and territories with continents
    if (anotherMap.territories != nullptr && anotherMap.numTerritories > 0 && anotherMap.continents != nullptr && anotherMap.numContinents > 0) {
        for (int i = 0; i < numTerritories; i++) {

            // associating adjacency
            if (anotherMap.territories[i] != nullptr && anotherMap.territories[i]->adjTerritories != nullptr && anotherMap.territories[i]->numAdjTerritories > 0) {
                for (int j = 0; j < anotherMap.territories[i]->numAdjTerritories; j++) {
                    int idx = -1;
                    for (int k = 0; k < anotherMap.numTerritories; k++) {
                        if (anotherMap.territories[i]->adjTerritories[j] == anotherMap.territories[k]) {
                            idx = k;
                            break;
                        }
                    }
                    if (idx > -1) {
                        territories[i]->adjTerritories[j] = territories[idx];
                    }
                }
            }

            // linking territories to continents
            if (anotherMap.territories[i] != nullptr && anotherMap.territories[i]->continent != nullptr) {
                int idx = -1;
                for (int j = 0; j < anotherMap.numContinents; j++) {
                    if (anotherMap.territories[i]->continent == anotherMap.continents[j]) {
                        idx = j;
                        break;
                    }
                }
                if (idx > -1) {
                    territories[i]->continent = continents[idx];
                }
            }
        }

        // linking continents to territories
        for (int i = 0; i < anotherMap.numContinents; i++) {
            if (anotherMap.continents[i] != nullptr && anotherMap.continents[i]->territories != nullptr && anotherMap.continents[i]->numTerritories > 0) {
                for (int j = 0; j < anotherMap.continents[i]->numTerritories; j++) {
                    int idx = -1;
                    for (int k = 0; k < anotherMap.numTerritories; k++) {
                        if (anotherMap.continents[i]->territories[j] == anotherMap.territories[k]) {
                            idx = k;
                            break;
                        }
                    }
                    if (idx > -1) {
                        continents[i]->territories[j] = territories[idx];
                    }
                }
            }
        }
    }
}

// overloading the assignment operator for Map
Map& Map::operator = (Map const &anotherMap) {
    territories = nullptr;
    continents = nullptr;
    numTerritories = anotherMap.numTerritories;
    numContinents = anotherMap.numContinents;

    // copying list of territories
    if (anotherMap.territories != nullptr && anotherMap.numTerritories > 0) {
        territories = new Territory*[numTerritories];
        for (int i = 0; i < numTerritories; i++) {
            territories[i] = new Territory(*(anotherMap.territories[i]));
        }
    }

    // copying list of continents
    if (anotherMap.continents != nullptr && anotherMap.numContinents > 0) {
        continents = new Continent*[numContinents];
        for (int i = 0; i < numContinents; i++) {
            continents[i] = new Continent(*(anotherMap.continents[i]));
        }
    }
    // linking adjacent territories, and territories with continents
    if (anotherMap.territories != nullptr && anotherMap.numTerritories > 0 && anotherMap.continents != nullptr && anotherMap.numContinents > 0) {
        for (int i = 0; i < numTerritories; i++) {

            // associating adjacency
            if (anotherMap.territories[i] != nullptr && anotherMap.territories[i]->adjTerritories != nullptr && anotherMap.territories[i]->numAdjTerritories > 0) {
                for (int j = 0; j < anotherMap.territories[i]->numAdjTerritories; j++) {
                    int idx = -1;
                    for (int k = 0; k < anotherMap.numTerritories; k++) {
                        if (anotherMap.territories[i]->adjTerritories[j] == anotherMap.territories[k]) {
                            idx = k;
                            break;
                        }
                    }
                    if (idx > -1) {
                        territories[i]->adjTerritories[j] = territories[idx];
                    }
                }
            }

            // linking territories to continents
            if (anotherMap.territories[i] != nullptr && anotherMap.territories[i]->continent != nullptr) {
                int idx = -1;
                for (int j = 0; j < anotherMap.numContinents; j++) {
                    if (anotherMap.territories[i]->continent == anotherMap.continents[j]) {
                        idx = j;
                        break;
                    }
                }
                if (idx > -1) {
                    territories[i]->continent = continents[idx];
                }
            }
        }

        // linking continents to territories
        for (int i = 0; i < anotherMap.numContinents; i++) {
            if (anotherMap.continents[i] != nullptr && anotherMap.continents[i]->territories != nullptr && anotherMap.continents[i]->numTerritories > 0) {
                for (int j = 0; j < anotherMap.continents[i]->numTerritories; j++) {
                    int idx = -1;
                    for (int k = 0; k < anotherMap.numTerritories; k++) {
                        if (anotherMap.continents[i]->territories[j] == anotherMap.territories[k]) {
                            idx = k;
                            break;
                        }
                    }
                    if (idx > -1) {
                        continents[i]->territories[j] = territories[idx];
                    }
                }
            }
        }
    }
    return *this;
}

// Map destructor
Map::~Map() {
    // deallocating territories
    if (territories != nullptr && numTerritories > 0) {
        for (int i = 0; i < numTerritories; i++) {
            delete *(territories + i);
            territories[i] = nullptr;
        }
        delete[] territories;
        territories = nullptr;
    }

    // deallocating continents
    if (continents != nullptr && numContinents > 0) {
        for (int i = 0; i < numContinents; i++) {
            delete *(continents + i);
            continents[i] = nullptr;
        }
        delete[] continents;
        continents = nullptr;
    }
}

// a method that checks whether a map is valid or not, and returns an int
// returns  0 if the map is valid
//          1 if the map is not a connected graph
//          2 if at least one continent in the map is not a connected sub-graph
//          3 if at least one territory belongs to more than one continent
int Map::validate() const {
    // checking whether the map is a connected graphs by recursively trying to reach the starting territory
    for (int i = 0; i < numTerritories; i++) {
        if (territories != nullptr && territories[i] != nullptr) {
            // setting an array for the search path
            Territory** path = new Territory*[numTerritories];
            path[0] = territories[i];
            for (int j = 1; j < numTerritories; j++) {
                path[j] = nullptr;
            }
            if (!checkConnectivity(territories[i], path, false)) {
                delete[] path;
                return 1;
            }
            delete[] path;
        }
    }

    // checking whether continents are connected sub-graphs by recursively trying to reach the starting territory within the same continent
    for (int i = 0; i < numContinents; i++) {
        if (continents != nullptr && continents[i] != nullptr) {
            for (int j = 0; j < continents[i]->numTerritories; j++) {
                if (continents[i]->territories != nullptr && continents[i]->territories[j] != nullptr) {
                    // setting an array for the search path
                    Territory** path = new Territory*[continents[i]->numTerritories];
                    path[0] = continents[i]->territories[j];
                    for (int k = 1; k < continents[i]->numTerritories; k++) {
                        path[k] = nullptr;
                    }
                    if (!checkConnectivity(continents[i]->territories[j], path, true)) {
                        delete[] path;
                        return 2;
                    }
                    delete[] path;
                }
            }
        }
    }

    // Checking whether a territory belongs to more than one continent by checking if a single territory can be reached from 2 different continents
    bool reachedTer[numTerritories];
    for (int i = 0; i < numTerritories; i++) {
        reachedTer[i] = false;
    }
    for (int i = 0; i < numContinents; i++) {
        if (continents != nullptr && continents[i] != nullptr) {
            for (int j = 0; j < continents[i]->numTerritories; j++) {
                if (continents[i]->territories != nullptr && continents[i]->territories[j] != nullptr) {
                    if (reachedTer[continents[i]->territories[j]->id - 1]) {
                        return 3;
                    }
                    reachedTer[continents[i]->territories[j]->id - 1] = true;
                }
            }
        }
    }

    // the map is valid
    return 0;
}

// a recursive function that searches and tries to reach the starting territory either within or out of the starting territory's continent
// the function search mechanism doesn't go to the same territory twice in the followed path
// parameters:  ter: a pointer to the starting territory
//              path: the followed path from ter
//              withinContinent: specifies wither to search only within the starting territory's continent or within all the map's territories
// returns: true if the starting territory reached again
//          false if it couldn't reach the starting territory
bool Map::checkConnectivity(Territory *ter, Territory **path, bool withinContinent) const {
    if (ter != nullptr && ter->continent != nullptr && ter->adjTerritories != nullptr && path != nullptr) {
        for (int i = 0; i < ter->numAdjTerritories; i++) {
            if (!withinContinent || (ter->adjTerritories[i] != nullptr && ter->adjTerritories[i]->continent == path[0]->continent)) {
                if (ter->adjTerritories[i] == path[0]) {
                    return true;
                }
                bool inPath(false);
                int indexOfFirstNull(-1);
                int lengthOfPath = withinContinent && path[0]->continent != nullptr ? path[0]->continent->numTerritories : numTerritories;
                for (int j = 1; j < lengthOfPath; j++) {
                    if (indexOfFirstNull < 0 && path[j] == nullptr) {
                        indexOfFirstNull = j;
                    }
                    if (path[j] == ter->adjTerritories[i]) {
                        inPath = true;
                        break;
                    }
                    if (!inPath && ter->adjTerritories[i] != nullptr && indexOfFirstNull > -1) {
                        path[indexOfFirstNull] = ter->adjTerritories[i];
                        if (checkConnectivity(ter->adjTerritories[i], path, withinContinent)) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

// Territory default constructor
Territory::Territory() {
    adjTerritories = nullptr;
    continent = nullptr;
    owner = nullptr;
    numArmies = 0;
    id = -1;
    numAdjTerritories = 0;
    name = "";
    pendingIncomingArmies = 0;
    pendingOutgoingArmies = 0;
}

// Territory parametrized constructor
Territory::Territory(int id, string name, int armies, Continent* continent) {
    adjTerritories = nullptr;
    this->continent = continent;
    owner = nullptr;
    this->numArmies = armies;
    this->id = id;
    numAdjTerritories = 0;
    this->name = name;
    pendingIncomingArmies = 0;
    pendingOutgoingArmies = 0;
}

// Territory copy constructor
Territory::Territory(Territory const &anotherTerritory) {
    id = anotherTerritory.id;
    name = anotherTerritory.name;
    owner = nullptr;
    numAdjTerritories = anotherTerritory.numAdjTerritories;
    numArmies = anotherTerritory.numArmies;
    continent = anotherTerritory.continent;
    pendingIncomingArmies = 0;
    pendingOutgoingArmies = 0;
    adjTerritories = nullptr;

//    // copying list of adjacent territories
//    if (anotherTerritory.adjTerritories != nullptr && anotherTerritory.numAdjTerritories > 0) {
//        adjTerritories = new Territory*[numAdjTerritories];
//        for (int i = 0; i < numAdjTerritories; i++) {
//            adjTerritories[i] = anotherTerritory.adjTerritories[i];
//        }
//    }

    // creating empty list of adjacent territories
    if (anotherTerritory.adjTerritories != nullptr && anotherTerritory.numAdjTerritories > 0) {
        adjTerritories = new Territory*[numAdjTerritories];
    }
}

// overloading the assignment operator for Territory
Territory& Territory::operator = (Territory const &anotherTerritory) {
    id = anotherTerritory.id;
    name = anotherTerritory.name;
    owner = nullptr;
    numAdjTerritories = anotherTerritory.numAdjTerritories;
    numArmies = anotherTerritory.numArmies;
    continent = anotherTerritory.continent;
    pendingIncomingArmies = 0;
    pendingOutgoingArmies = 0;
    adjTerritories = nullptr;

//    // copying list of adjacent territories
//    if (anotherTerritory.adjTerritories != nullptr && anotherTerritory.numAdjTerritories > 0) {
//        adjTerritories = new Territory*[numAdjTerritories];
//        for (int i = 0; i < numAdjTerritories; i++) {
//            adjTerritories[i] = anotherTerritory.adjTerritories[i];
//        }
//    }

    // creating empty list of adjacent territories
    if (anotherTerritory.adjTerritories != nullptr && anotherTerritory.numAdjTerritories > 0) {
        adjTerritories = new Territory*[numAdjTerritories];
        for (int i = 0; i < numAdjTerritories; i++) {
            adjTerritories[i] = nullptr;
        }
    }
    return *this;
}

// Territory destructor
Territory::~Territory() {
    // removing any reference to this territory in the adjacent territories, then deallocating memory reserved to list of adjacent territories
    if (adjTerritories != nullptr && numAdjTerritories > 0) {
        for (int i = 0; i < numAdjTerritories; i++) {
            if (adjTerritories[i] != nullptr) {
                Territory* adjTer = *(adjTerritories + i);
                if (adjTer != nullptr) {
                    int numTerAdjTer = adjTer->numAdjTerritories;
                    for (int j = 0; j < numTerAdjTer; j++) {
                        if (adjTer->adjTerritories != nullptr && adjTer->adjTerritories[j] != nullptr && adjTer->adjTerritories[j] == this) {
                            adjTer->adjTerritories[j] = nullptr;
                        }
                    }
                }
            }
            *(adjTerritories + i) = nullptr;
        }
        delete[] adjTerritories;
        adjTerritories = nullptr;
    }

    // remove any reference to this territory in the containing continent
    if (continent != nullptr) {
        for (int i = 0; i < continent->numTerritories; i++) {
            if (continent->territories != nullptr && continent->territories[i] != nullptr && continent->territories[i]->id == id) {
                continent->territories[i] = nullptr;
            }
        }
        continent = nullptr;
    }

    // remove any reference to this territory in its owner's list of territories
    if (owner != nullptr) {
        owner->removeTerritory(this);
        owner = nullptr;
    }
}

// mutator that sets ownership of territory to a player
void Territory::setOwner(Player *owner) {
    this->owner = owner;

    // set ownership of the containing continent when player owns all territories in a continent
    bool ownContinent = true;
    if (continent != nullptr) {
        for (int i = 0; i < continent->numTerritories; i++) {
            if (continent->territories != nullptr && continent->territories[i] != nullptr && continent->territories[i]->owner != owner) {
                ownContinent = false;
                break;
            }
        }
        if (ownContinent) {
            continent->owner = owner;
        }
    }
}

// accessor to owner of territory
Player* Territory::getOwner() const {
    return owner;
}

// mutator to set number of armies in territory
void Territory::setNumberOfArmies(int newArmies) {
    numArmies = newArmies;
}

// accessor to get number of armies in territory
int Territory::getNumberOfArmies() const {
    return numArmies;
}

// accessor to get id of territory
int Territory::getId() const {
    return id;
}

// method that removes ownership of territory, and removes ownership of continent if belongs to the same owner
void Territory::removeOwner() {
    if (continent != nullptr && continent->owner == owner) {
        continent->owner = nullptr;
    }
    owner = nullptr;
}

// mutator that sets number and list of adjacent territories
void Territory::setAdjTerritories(int numAdjTer, Territory **adjTers) {
    if (adjTerritories != nullptr && numAdjTerritories > 0) {
        delete[] adjTerritories;
    }
    numAdjTerritories = numAdjTer;
    adjTerritories = adjTers;
}

// method that adds number of armies to territory
void Territory::addArmies(int armies) {
    numArmies += armies;
}

// accessor to get number of pending incoming armies
int Territory::getPendingIncomingArmies() const {
    return pendingIncomingArmies;
}

// accessor to get number of pending outgoing armies
int Territory::getPendingOutgoingArmies() const {
    return pendingOutgoingArmies;
}

// accessor to get name of territory
string Territory::getName() const {
    return name;
}

// mutator to set number of pending incoming armies
void Territory::setPendingIncomingArmies(int armies) {
    pendingIncomingArmies = armies;
}

// mutator to set number of pending outgoing armies
void Territory::setPendingOutgoingArmies(int armies) {
    pendingOutgoingArmies = armies;
}

// function that removes number of armies from territory
void Territory::removeArmies(int armies) {
    numArmies -= armies;
    if (numArmies < 0)
    {
        numArmies = 0;
    }
}

// function that adds number of armies to pending incoming armies
void Territory::addPendingIncomingArmies(int armies) {
    pendingIncomingArmies += armies;
}

// function that adds number of armies to pending outgoing armies
void Territory::addPendingOutgoingArmies(int armies) {
    pendingOutgoingArmies += armies;
}

// Get the number of armies on the territory that are available for moving (advance/airlift).
// This number represents the armies already present + the incoming armies from deployment - the armies that will be used for an advance/airlift.
int Territory::getNumberOfMovableArmies() const {
    return numArmies + pendingIncomingArmies - pendingOutgoingArmies;
}

// Returns the containing continent
Continent * Territory::getContinent() const {
    return continent;
}

// Continent default constructor
Continent::Continent() {
    id = -1;
    name = "";
    bonus = 0;
    territories = nullptr;
    numTerritories = 0;
    owner = nullptr;
}

// Continent parametrized constructor
Continent::Continent(int id, string name, int bonus) {
    this->id = id;
    this->name = name;
    this->bonus = bonus;
    territories = nullptr;
    numTerritories = 0;
    owner = nullptr;
}

// Continent copy constructor
Continent::Continent(Continent const &anotherContinent) {
    id = anotherContinent.id;
    name = anotherContinent.name;
    bonus = anotherContinent.bonus;
    owner = nullptr;
    numTerritories = anotherContinent.numTerritories;
    territories = nullptr;

    // creating empty list of territories
    if (anotherContinent.territories != nullptr && anotherContinent.numTerritories > 0) {
        territories = new Territory*[numTerritories];
        for (int i = 0; i < numTerritories; i++) {
            territories[i] = nullptr;
        }
    }
}

// overloading the assignment operator for Continent
Continent& Continent::operator = (Continent const &anotherContinent) {
    id = anotherContinent.id;
    name = anotherContinent.name;
    bonus = anotherContinent.bonus;
    owner = nullptr;
    numTerritories = anotherContinent.numTerritories;
    territories = nullptr;

    // creating empty list of territories
    if (anotherContinent.territories != nullptr && anotherContinent.numTerritories > 0) {
        territories = new Territory*[numTerritories];
    }

    return *this;
}

// Continent destructor
Continent::~Continent() {
    // removing any reference to this continent in its territories, then deallocating memory reserved to list of territories
    if (territories != nullptr && numTerritories > 0) {
        for (int i = 0; i < numTerritories; i++) {
            if (territories[i] != nullptr && territories[i]->continent != nullptr && territories[i]->continent->id == id) {
                territories[i] = nullptr;
            }
        }
        delete[] territories;
        territories = nullptr;
    }
    owner = nullptr;
}

// accessor to get name of the owner of this continent
Player* Continent::getOwner() const{
    return owner;
}

// mutator that set number and list of territories
void Continent::setTerritories(int numTers, Territory** ters) {
    if (territories != nullptr && numTerritories > 0) {
        delete[] territories;
    }
    numTerritories = numTers;
    territories = ters;
}

// accessor to get name of this continent
string Continent::getName() const {
    return string(name);
}


ostream& operator << (ostream &stream, const MapLoader &mapLoader) {
    // do nothing
    return stream;
}


ostream& operator << (ostream &stream, const Map &map) {
    stream << "Number of Territories: " << map.numTerritories;
    stream << "\nNumber of Continents: " << map.numContinents;
    stream << "\n\nContinents:";
    if (map.continents != nullptr) {
        for (int i = 0; i < map.numContinents; i++) {
            if (map.continents[i] != nullptr) {
                stream << "\n" << *(map.continents[i]);
            }
        }
    }
    return stream;
}

ostream& operator << (ostream &stream, const Territory &territory) {
    stream << "ID: " << territory.id << ", Name: " << territory.name << ", Number of armies: " << territory.numArmies;
    if (territory.continent != nullptr) {
        stream << ", Continent: " << territory.continent->getName();
    }
    if (territory.owner != nullptr) {
        stream << ", Owner's Name: " << territory.owner->getName();
    }
    stream << ", Number of adjacent territories: " << territory.numAdjTerritories;
    if (territory.adjTerritories != nullptr && territory.numAdjTerritories > 0) {
        stream << ", adjacent territories are:";
        for (int i = 0; i < territory.numAdjTerritories; i++) {
            if (territory.adjTerritories[i] != nullptr) {
                stream << "\n\t\t" << territory.adjTerritories[i]->name;
                if (i < territory.numAdjTerritories - 1) {
                    stream << ", ";
                }
            }
        }
    }
    return stream;
}
///////////////////////////////////////////////////////////////////////////////Sarah
int Territory::getNumAdjTerritories() const {
    return numAdjTerritories;
}

Territory **Territory::getAdjTerritories() const {
    return adjTerritories;
}
/////////////////////////////////////////////////////////////////////////////////


ostream& operator << (ostream& stream, const Continent& continent) {
    stream << "ID: " << continent.id << ", Name: " << continent.name << ", Number of territories: " << continent.numTerritories << ", Bonus: " << continent.bonus;
    if (continent.owner != nullptr) {
        stream << ", Owner's Name: " << continent.owner->getName();
    }
    if (continent.territories != nullptr) {
        stream << "\n\tTerritories: ";
        if (continent.territories != nullptr && continent.numTerritories > 0) {
            for (int i = 0; i < continent.numTerritories; i++) {
                // add territories to the stream
                if (continent.territories[i] != nullptr) {
                    stream << "\n\t" << *(continent.territories[i]);
                }
            }
        }
    }
    return stream;
}
//////////////////////////////////////////////////////////sarah
int Continent::getBonus() const {
    return bonus;
}
//////////////////////////////////////////////////////////////////////

////// for demo purpose only, to be removed later
void Map::setOwnerOfTerritory(Player *player) {
    for (int i = 0; i < continents[0]->numTerritories; i++) {
        continents[0]->territories[i]->setOwner(player);
    }
}

////// for demo purpose only, to be removed later
void Map::makeContinentContain2Territories() {
    continents[0]->territories[1] = continents[1]->territories[0];
    continents[0]->territories[0]->numArmies = 111;
}

/////////////////////////////////////////////////////////////SARAH
int Map::getNumContinent() {
    return this->numContinents;
}

Continent **Map::getContinent() const {
    return this->continents;
}
/////////////////////////////////////////////////////////////////////

int Map::getNumTerritories() {
    return numTerritories;
}

Territory **Map::getTerritories() const {
    return territories;
}