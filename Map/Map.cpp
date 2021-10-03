//
// Created by fadib on 2021-09-25.
//

#include "Map.h"
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

MapLoader::MapLoader() {
    map = new Map;
}

MapLoader::MapLoader(string fileName) {
    map = new Map;
    loadFromFile(fileName);
}

void MapLoader::loadFromFile(std::string fileName) {
    ifstream mapFile(fileName);
    string line;
    stringstream strStream;
    regex regexSection("(\\[)(.+)(\\])");
    regex regexLine;
    smatch strMatch;
    int numContinents = 0;
    int numCountries = 0;
    int* borders = NULL;
    Continent* continents;
    Territory* territories;
    // looping on map file twice:
    // no.1 - to get number of continents, territories ...
    // no.2 - to instantiate objects of continents, territories ...
    for(int i = 0; i < 2; i++) {
        bool readFlag(false);
        bool regexFlag(false);
        string section;
        if (mapFile.eof()) {
            mapFile.clear();
            mapFile.seekg(0, mapFile.beg);
        }
        while (!mapFile.eof() && !mapFile.fail()) {
            getline(mapFile, line);
//            strStream << line;
            regexFlag = regex_match(line, strMatch, regexSection);
            if (regexFlag) {
                //            cout << "match: ====" << strMatch.str() << " - " << strMatch[2].str() << endl;
                section = strMatch[2].str();
                if(section == "continents") {
                    regexLine = regex("");
                } else if (section == "borders" ) {
//                    cout << "match: " << strMatch.str() << " - " << strMatch[2] << endl;
                    borders = new int[numCountries];
                    for(int j = 0; j < numCountries; j++) {
                        borders[j] = 0;
                    }
                    regexLine = regex("");
                } else if (section == "borders" ) {
                    regexLine = regex("");
                }
            }
            if (!regexFlag && line.length() > 0) {
                strStream << line;
                if(section == "continents") {
                    if(i==0){
                        numContinents++;
                    } else {
                        // instantiate objects
                        cout << "continent: " << line << endl;
                    }
                } else if(section == "countries") {
                    if(i==0){
                        numCountries++;
                    } else {
                        // instantiate objects
                        cout << "country: " << line << endl;
                    }
                } else if(section == "borders") {
                    if(i==0){
                        borders = new int[numCountries];
                    } else {
                        // instantiate objects
                        cout << "boarder: " << line << endl;
                    }
                }
//                cout << "caught: " << line << endl;
            }
            //        cout << line << endl;
        }
    }
    delete[] borders;
    borders = NULL;
    mapFile.clear();
    mapFile.seekg(0, mapFile.beg);
//    mapFile.close();
//    mapFile.open(fileName);
    getline(mapFile, line);
    cout << "after reset: " << line << endl;
    getline(mapFile, line);
    cout << "after reset: " << line << endl;
    mapFile.close();
}

Map::Map() {
    territory = NULL;
}

Territory::Territory() {
    adjTerritory = NULL;
}

Territory::Territory(int id, string name, int army) {
    this->id =id;
    this->name = name;
    this->army = army;

}

ostream &operator<<(ostream &out, const Territory &ter) {
    out<<"territory "<< ter.id<< " ,"<<ter.name<<endl;
    return out;
}
