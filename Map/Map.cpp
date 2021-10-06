//
// Created by Fadi Albasha on 2021-09-25.
//

#include "../Map/Map.h"
//#include "../Player/Player.h"
//#include <string>
//#include <regex>
//#include <fstream>
////#include <sstream>
//#include <iostream>
//using namespace std;

MapLoader::MapLoader() {
    territories = nullptr;
    continents = nullptr;
    numContinents = 0;
    numTerritories = 0;
}

MapLoader::MapLoader(string fileName) {
    territories = nullptr;
    continents = nullptr;
    numContinents = 0;
    numTerritories = 0;
    loadFromFile(fileName);
}

MapLoader::MapLoader(MapLoader const &anotherMapLoader) {
    territories = nullptr;
    continents = nullptr;
    numTerritories = anotherMapLoader.numTerritories;
    numContinents = anotherMapLoader.numContinents;
    if (anotherMapLoader.territories != nullptr && anotherMapLoader.numTerritories > 0) {
        territories = new Territory*[numTerritories];
        for (int i = 0; i < numTerritories; i++) {
            territories[i] = anotherMapLoader.territories[i];
        }
    }
    if (anotherMapLoader.continents != nullptr && anotherMapLoader.numContinents > 0) {
        continents = new Continent*[numContinents];
        for (int i = 0; i < numContinents; i++) {
            continents[i] = anotherMapLoader.continents[i];
        }
    }
}

MapLoader& MapLoader::operator = (MapLoader const &anotherMapLoader) {
    this->territories = nullptr;
    this->continents = nullptr;
    this->numTerritories = anotherMapLoader.numTerritories;
    this->numContinents = anotherMapLoader.numContinents;
    if (anotherMapLoader.territories != nullptr && anotherMapLoader.numTerritories > 0) {
        this->territories = new Territory*[numTerritories];
        for (int i = 0; i < numTerritories; i++) {
            this->territories[i] = anotherMapLoader.territories[i];
        }
    }
    if (anotherMapLoader.continents != nullptr && anotherMapLoader.numContinents > 0) {
        this->continents = new Continent*[numContinents];
        for (int i = 0; i < numContinents; i++) {
            this->continents[i] = anotherMapLoader.continents[i];
        }
    }
    return *this;
}

MapLoader::~MapLoader() {
    cout << "enter ~MapLoader()" << endl;
    // deallocating territories
    if (territories != nullptr && numTerritories > 0) {
        cout << "~MapLoader() - non-null territories" << endl;
        for (int i = 0; i < numTerritories; i++) {
            cout << "~MapLoader() - deleting ter no." << i+1 << ": " << (*(territories + i))->name << endl;
            delete *(territories + i);
            territories[i] = nullptr;
//            delete (territories + i);
        }
        delete[] territories;
        cout << "~MapLoader() - setting territories to null" << endl;
        territories = nullptr;
    }
    // deallocating continents
    if (continents != nullptr && numContinents > 0) {
        for (int i = 0; i < numContinents; i++) {
            delete *(continents + i);
            continents[i] = nullptr;
//            delete (continents + i);
        }
        delete[] continents;
        continents = nullptr;
    }
    cout << "exit ~MapLoader()" << endl;
}

void MapLoader::loadFromFile(std::string fileName) {
    ifstream mapFile(fileName);
    string line;
    stringstream strStream;
    regex regexSection("(\\[)(.+)(\\])");
    regex regexLine;
    smatch strMatch;
    numContinents = 0;
    numTerritories = 0;
    int indexContinents = 0;
    int indexCountries = 0;
    int* borders = nullptr;
    int* numCntryInContnt;
//    Continent** continents;
//    Territory** territories;
    // looping on map file twice:
    // iter. no.1 - to get number of continents, territories ...
    // iter. no.2 - to instantiate objects of continents, territories ...
    for(int i = 0; i < 2; i++) {
        bool readFlag(false);
        bool regexFlag(false);
        string section("");
        string prevSection("");
        int prevContntId = 0;
        int cntryInContnt = 0;
        if (mapFile.eof()) {
            mapFile.clear();
            mapFile.seekg(0, ifstream::beg);
        }
        while (!mapFile.eof() && !mapFile.fail()) {
            getline(mapFile, line);
//            strStream << line;
            regexFlag = regex_match(line, strMatch, regexSection);
            if (regexFlag) {
                //            cout << "match: ====" << strMatch.str() << " - " << strMatch[2].str() << endl;
                prevSection = section;
                section = strMatch[2].str();

            }
            // After end of a section, reset and create arrays if needed
            if (i==0 && line.length() == 0 && section != "") {
                if (section == "continents") {
                    if (numContinents > 0) {
                        continents = new Continent*[numContinents];
                        numCntryInContnt = new int[numContinents];
                        for(int j = 0; j < numContinents; j++) {
                            numCntryInContnt[j] = 0;
                        }
                    }
                    regexLine = regex("");
                } else if (section == "countries") {
                    //                    cout << "match: " << strMatch.str() << " - " << strMatch[2] << endl;
                    if (numTerritories > 0) {
                        territories = new Territory*[numTerritories];
                        borders = new int[numTerritories];
                        for (int j = 0; j < numTerritories; j++) {
                            borders[j] = 0;
                        }
                    }
                    regexLine = regex("");
                } else if (section == "borders") {

                    regexLine = regex("");
                }
                section = "";
            }
            if (!regexFlag && line.length() > 0 && !section.empty()) {
                strStream.str(line);
                strStream.clear();
                if(section == "continents") {
                    if(i==0) {
                        numContinents++;
                    } else {
                        // instantiate objects
                        string name;
                        int bonus;
                        string color;
                        if (strStream >> name >> bonus >> color) {
                            continents[indexContinents] = new Continent(indexContinents+1, name, bonus);
                        } else {
                            continents[indexContinents] = nullptr;
                        }
                        indexContinents++;
                        cout << "continent: " << line << endl;
                    }
                } else if(section == "countries") {
                    if(i==0) {
                        numTerritories++;
                    } else {
                        // instantiate objects
                        int id;
                        string name;
                        int continentId;
                        int x, y;
                        if (strStream >> id >> name >> continentId >> x >> y) {
                            if (indexCountries == id - 1) {
                                territories[indexCountries] = new Territory(id, name, 0, continents[continentId - 1]);
                            } else {
                                territories[indexCountries] = nullptr;
                            }
                        } else {
                            territories[indexCountries] = nullptr;
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
                        if (indexCountries == numTerritories && continentId > 0) {
                            numCntryInContnt[continentId-1] = cntryInContnt;
                        }
                        if (indexCountries == numTerritories) {

                            for(int j = 0; j < numContinents; j++) {
                                Territory** ter = new Territory*[numCntryInContnt[j]];
                                for (int k = 0; k < numCntryInContnt[j]; k++) {
                                    ter[k] = nullptr;
                                }
                                int index = 0;
                                for (int k = 0; k < numTerritories; k++) {
                                    if (territories[k]->continent->id == (j+1)){
                                        ter[index++] = territories[k];
                                    }
                                }
                                continents[j]->territories = ter;
                                continents[j]->numTerritories = numCntryInContnt[j];
                            }
                        }
                        cout << "country: " << line << endl;
                    }
                } else if(section == "borders") {
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
                            // instantiate objects
                            Territory** adjTer = new Territory*[borders[terId-1]];
                            for (int j = 0; j < borders[terId-1]; j++) {
                                adjTer[j] = nullptr;
                            }
                            int index = 0;
                            while(strStream >> adjTerId) {
                                if (adjTerId > 0) {
                                    adjTer[index++] = *(territories + adjTerId - 1);
                                }
                            }
                            territories[terId-1]->adjTerritories = adjTer;
                            territories[terId-1]->numAdjTerritories = borders[terId-1];

                            cout << "boarder: " << line << endl;
                        }

                    }
                }
//                cout << "caught: " << line << endl;
            }
            //        cout << line << endl;
        }
    }
    /////////////////// debugging
    cout << "Continents: " << endl;
    for (int i = 0; i < numContinents; i++) {
        cout << *(continents[i]) << endl;
    }
    cout << "end of debug" << endl;
    ///////////////////
    delete[] borders;
    borders = nullptr;
    delete[] numCntryInContnt;
    numCntryInContnt = nullptr;
    mapFile.clear();
    mapFile.seekg(0, std::ifstream::beg);
//    mapFile.close();
//    mapFile.open(fileName);
    getline(mapFile, line);
    cout << "after reset: " << line << endl;
    getline(mapFile, line);
    cout << "after reset: " << line << endl;
    mapFile.close();
}

Map::Map() {
    territories = nullptr;
    continents = nullptr;
    numTerritories = 0;
    numContinents = 0;
}


Map::Map(Map const &anotherMap) {
    territories = nullptr;
    continents = nullptr;
    numTerritories = anotherMap.numTerritories;
    numContinents = anotherMap.numContinents;
    if (anotherMap.territories != nullptr && anotherMap.numTerritories > 0) {
        territories = new Territory*[numTerritories];
        for (int i = 0; i < numTerritories; i++) {
            territories[i] = anotherMap.territories[i];
        }
    }
    if (anotherMap.continents != nullptr && anotherMap.numContinents > 0) {
        continents = new Continent*[numContinents];
        for (int i = 0; i < numContinents; i++) {
            continents[i] = anotherMap.continents[i];
        }
    }
}

Map& Map::operator = (Map const &anotherMap) {
    territories = nullptr;
    continents = nullptr;
    numTerritories = anotherMap.numTerritories;
    numContinents = anotherMap.numContinents;
    if (anotherMap.territories != nullptr && anotherMap.numTerritories > 0) {
        territories = new Territory*[numTerritories];
        for (int i = 0; i < numTerritories; i++) {
            territories[i] = anotherMap.territories[i];
        }
    }
    if (anotherMap.continents != nullptr && anotherMap.numContinents > 0) {
        continents = new Continent*[numContinents];
        for (int i = 0; i < numContinents; i++) {
            continents[i] = anotherMap.continents[i];
        }
    }
    return *this;
}

Map::Map(MapLoader const &mapLoader) {
    territories = mapLoader.territories;
    continents = mapLoader.continents;
    numTerritories = mapLoader.numTerritories;
    numContinents = mapLoader.numContinents;
}

Map::~Map() {
    cout << "enter ~Map()" << endl;
    // deallocating territories
    if (territories != nullptr && numTerritories > 0) {
        for (int i = 0; i < numTerritories; i++) {
            delete *(territories + i);
            territories[i] = nullptr;
//            delete (territories + i);
        }
        delete[] territories;
        territories = nullptr;
    }
    // deallocating continents
    if (continents != nullptr && numContinents > 0) {
        for (int i = 0; i < numContinents; i++) {
            delete *(continents + i);
            continents[i] = nullptr;
//            delete (continents + i);
        }
        delete[] continents;
        continents = nullptr;
    }
    cout << "exit ~Map()" << endl;
}

Territory::Territory() {
    adjTerritories = nullptr;
    continent = nullptr;
    owner = nullptr;
    armies = 0;
    id = -1;
    numAdjTerritories = 0;
    name = "";
}

Territory::Territory(int id, string name, int armies, Continent* continent) {
    adjTerritories = nullptr;
    this->continent = continent;
    owner = nullptr;
    this->armies = armies;
    this->id = id;
    numAdjTerritories = 0;
    this->name = name;
}

Territory::Territory(Territory const &anotherTerritory) {
    id = anotherTerritory.id;
    name = anotherTerritory.name;
    numAdjTerritories = anotherTerritory.numAdjTerritories;
    armies = anotherTerritory.armies;
    continent = anotherTerritory.continent;
    adjTerritories = nullptr;
    if (anotherTerritory.adjTerritories != nullptr && anotherTerritory.numAdjTerritories > 0) {
        adjTerritories = new Territory*[numAdjTerritories];
        for (int i = 0; i < numAdjTerritories; i++) {
            adjTerritories[i] = anotherTerritory.adjTerritories[i];
        }
    }
}

Territory& Territory::operator = (Territory const &anotherTerritory) {
    id = anotherTerritory.id;
    name = anotherTerritory.name;
    numAdjTerritories = anotherTerritory.numAdjTerritories;
    armies = anotherTerritory.armies;
    continent = anotherTerritory.continent;
    adjTerritories = nullptr;
    if (anotherTerritory.adjTerritories != nullptr && anotherTerritory.numAdjTerritories > 0) {
        adjTerritories = new Territory*[numAdjTerritories];
        for (int i = 0; i < numAdjTerritories; i++) {
            adjTerritories[i] = anotherTerritory.adjTerritories[i];
        }
    }
    return *this;
}

Territory::~Territory() {
    cout << "enter ~Territory(): " << name << endl;
    // deallocating territories
    if (adjTerritories != nullptr && numAdjTerritories > 0) {
        cout << "~Territory() - non-null adjTerritories " << endl;
        for (int i = 0; i < numAdjTerritories; i++) {
            cout << "~Territory() - adj.ter. no." << i+1 << endl;
            if (adjTerritories[i] != nullptr) {
                cout << "~Territory()->adjTerritory - non-null territory " << endl;
                cout << "~Territory() - adj.ter. no." << i+1 << ": " << (*(adjTerritories + i))->name << endl;
                Territory* adjTer = *(adjTerritories + i);
                int numTerAdjTer = adjTer->numAdjTerritories;
                for (int j = 0; j < numTerAdjTer; j++) {
                    cout << "~Territory()->adjTerritory no." << j+1 << endl;
                    if (adjTer->adjTerritories[j] != nullptr && adjTer->adjTerritories[j]->id == id) {
                        cout << "~Territory()->adjTerritory no." << j+1 << ": " << (adjTer->adjTerritories[j])->name << endl;
                        cout << "~Territory()->adjTerritory setting to null adj.ter. no." << j+1 << ": " << adjTer->name << endl;
                        adjTer->adjTerritories[j] = nullptr;
                    }
                }
            }
//            delete *(adjTerritories + i);
//            delete (adjTerritories + i);
            *(adjTerritories + i) = nullptr;
        }
//        delete[] adjTerritories;
        adjTerritories = nullptr;
    }
//    delete continent;
    if (continent != nullptr) {
        for (int i = 0; i < continent->numTerritories; i++) {
            if (continent->territories[i] != nullptr && continent->territories[i]->id == id) {
                continent->territories[i] = nullptr;
            }
        }
    }
    continent = nullptr;

    owner->removeTerritory(id);
    owner = nullptr;
    cout << "exit ~Territory()" << endl;
}

void Territory::setOwner(Player *owner) {
    this->owner = owner;
    bool ownContinent = true;
    if (continent != nullptr) {
        for (int i = 0; i < continent->numTerritories; i++) {
            if (continent->territories[i] != nullptr && continent->territories[i]->owner != owner) {
                ownContinent = false;
                break;
            }
        }
        if (ownContinent) {
            continent->owner = owner;
        }
    }
}

Player* Territory::getOwner() {
    return owner;
}

void Territory::setArmies(int newArmies) {
    armies = newArmies;
}

int Territory::getArmies() {
    return armies;
}

int Territory::getId() {
    return id;
}

void Territory::removeOwner() {
    if (continent != nullptr && continent->owner == owner) {
        continent->owner = nullptr;
    }
    owner = nullptr;
}

void Territory::addAdjTerritories(int numAdjTer, Territory **adjTers) {
    this->numAdjTerritories = numAdjTer;
    this->adjTerritories = adjTers;
}

// Add a number of armies to the current territory
void Territory::addArmies(int armies)
{
    numberOfArmies_ += armies;
}
int Territory::getNumberOfArmies() const
{
    return numberOfArmies_;
}

int Territory::getPendingIncomingArmies() const
{
    return pendingIncomingArmies_;
}

int Territory::getPendingOutgoingArmies() const
{
    return pendingOutgoingArmies_;
}

string Territory::getName() const
{
    return name_;
}

// Setters
void Territory::setName(std::string name)
{
    name_ = name;
}

void Territory::setPendingIncomingArmies(int armies)
{
    pendingIncomingArmies_ = armies;
}

void Territory::setPendingOutgoingArmies(int armies)
{
    pendingOutgoingArmies_ = armies;
}

// Remove a number of armies to the current territory
void Territory::removeArmies(int armies)
{
    numberOfArmies_ -= armies;
    if (numberOfArmies_ < 0)
    {
        numberOfArmies_ = 0;
    }
}

// Add a number of armies pending deployment to the current territory
void Territory::addPendingIncomingArmies(int armies)
{
    pendingIncomingArmies_ += armies;
}

// Mark a number of armies that will be moving off the current territory
void Territory::addPendingOutgoingArmies(int armies)
{
    pendingOutgoingArmies_ += armies;
}

// Get the number of armies on the territory that are available for moving (advance/airlift).
// This number represents the armies already present + the incoming armies from deployment - the armies that will be used for an advance/airlift.
int Territory::getNumberOfMovableArmies()
{
    return numberOfArmies_ + pendingIncomingArmies_ - pendingOutgoingArmies_;
}

Continent::Continent() {
    id = -1;
    name = "";
    bonus = 0;
    territories = nullptr;
    numTerritories = 0;
    owner = nullptr;
}

Continent::Continent(int id, string name, int bonus) {
    this->id = id;
    this->name = name;
    this->bonus = bonus;
    territories = nullptr;
    numTerritories = 0;
    owner = nullptr;
}

Continent::Continent(Continent const &anotherContinent) {
    id = anotherContinent.id;
    name = anotherContinent.name;
    bonus = anotherContinent.bonus;
    numTerritories = anotherContinent.numTerritories;
    territories = nullptr;
    if (anotherContinent.territories != nullptr && anotherContinent.numTerritories > 0) {
        territories = new Territory*[numTerritories];
        for (int i = 0; i < numTerritories; i++) {
            territories[i] = anotherContinent.territories[i];
        }
    }
}

Continent& Continent::operator = (Continent const &anotherContinent) {
    id = anotherContinent.id;
    name = anotherContinent.name;
    bonus = anotherContinent.bonus;
    numTerritories = anotherContinent.numTerritories;
    territories = nullptr;
    if (anotherContinent.territories != nullptr && anotherContinent.numTerritories > 0) {
        territories = new Territory*[anotherContinent.numTerritories];
        for (int i = 0; i < anotherContinent.numTerritories; i++) {
            territories[i] = anotherContinent.territories[i];
        }
    }
    return *this;
}

Continent::~Continent() {
    cout << "enter ~Continent(): " << name << endl;
    // deallocating territories
    if (territories != nullptr && numTerritories > 0) {
        for (int i = 0; i < numTerritories; i++) {
            if (territories[i] != nullptr && territories[i]->continent->id == id) {
                territories[i] = nullptr;
            }
//            delete *(territories + i);
//            delete (territories + i);
        }
        delete[] territories;
        territories = nullptr;
    }
    owner = nullptr;
    cout << "exit ~Continent()" << endl;
}

Player* Continent::getOwner() {
    return owner;
}

void Continent::setTerritories(int numTerritories, Territory** ters) {
    this->numTerritories = numTerritories;
    this->territories = ters;
}

string Continent::getName() {
    return string(name);
}

ostream& operator << (ostream &stream, const MapLoader &mapLoader) {
    stream << "Number of Territories: " << mapLoader.numTerritories;
    stream << "\nNumber of Continents: " << mapLoader.numContinents;
    return stream;
}

ostream& operator << (ostream &stream, const Map &map) {
    stream << "Number of Territories: " << map.numTerritories;
    stream << "\nNumber of Continents: " << map.numContinents;
    stream << "Continents: ";
    for (int i = 0; i < map.numContinents; i++) {
        stream << "\n" << *(map.continents[i]);
    }
    return stream;
}

ostream& operator << (ostream &stream, const Territory &territory) {
    stream << "ID: " << territory.id << ", Name: " << territory.name << ", Number of armies: " << territory.armies;
    stream << ", Continent: " << territory.continent->getName();
    stream << ", Number of adjacent territories: " << territory.numAdjTerritories;
    return stream;
}

ostream& operator << (ostream& stream, const Continent& continent) {
    stream << "ID: " << continent.id << ", Name: " << continent.name << ", Bonus: " << continent.bonus;
    if (continent.territories != nullptr) {
        stream << "\n\tTerritories: ";
        for (int i = 0; i < continent.numTerritories; i++) {
            // print territories
            stream << "\n\t" << *(continent.territories[i]);
        }
    }
    return stream;
}
