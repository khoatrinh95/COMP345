//
// Created by fadib on 2021-09-26.
//

#include "MapDriver.h"
class player;

void mapDriver() {
    // default constructor
    Map* map0 = new Map();
    cout << "\nMap 0: Default Constructor\n" << *map0 << endl;
    delete map0;

    // reading canada.map
    Map* map = MapLoader::loadMapFile("../Map/maps/canada.map");
    Player* player = new Player("Fadi"); ////// for demo purpose only, not deleting the object after. I understand it causes memory leak, but since it's only part of the demo and will be deleted later. the reason is, in my driver, I only create a dummy object of type Player because the prof said we don't need to link the classes for this assignment submission.
    map->setOwnerOfTerritory(player);
    cout << "\n\n\n\nMap 1: Canada\n" << *map << endl;
    cout << "\nValidating the map... ";
    int validation = map->validate();
    switch (validation) {
        case 0:
            cout << "The map is valid." << endl;
            break;
        case 1:
            cout << "The map is not a connected graph" << endl;
            break;
        case 2:
            cout << "At least one continents is not a connected sub-graph" << endl;
            break;
        case 3:
            cout << "At least one territory belongs to more than one continent" << endl;
            break;
        default:
            cout << "Unknown issue!" << endl;
    }

    // copying map 1
    Map* map2 = new Map(*map);

    // mutating map 1
    map->makeContinentContain2Territories();

    // re-printing and validating map 1 after making 2 territories belong to the same continent
    cout << "\n\n\n\nMap 1: Canada (after making 2 territories belong to the same continent)\n" << *map << endl;
    cout << "\nValidating the map... ";
    validation = map->validate();
    switch (validation) {
        case 0:
            cout << "The map is valid." << endl;
            break;
        case 1:
            cout << "The map is not a connected graph" << endl;
            break;
        case 2:
            cout << "At least one continents is not a connected sub-graph" << endl;
            break;
        case 3:
            cout << "At least one territory belongs to more than one continent" << endl;
            break;
        default:
            cout << "Unknown issue!" << endl;
    }

    delete map;
    map = nullptr;

    cout << "\n\n\n\nMap 2 Copy of Canada:\n" << *map2 << endl;
    cout << "\nValidating the map... ";
    validation = map2->validate();
    switch (validation) {
        case 0:
            cout << "The map is valid." << endl;
            break;
        case 1:
            cout << "The map is not a connected graph" << endl;
            break;
        case 2:
            cout << "At least one continents is not a connected sub-graph" << endl;
            break;
        case 3:
            cout << "At least one territory belongs to more than one continent" << endl;
            break;
        default:
            cout << "Unknown issue!" << endl;
    }
    delete map2;
    map2 = nullptr;

    Map* map3 = MapLoader::loadMapFile("../Map/maps/europe.map");
    cout << "\n\n\n\nMap 3: Europe\n" << *map3 << endl;
    cout << "\nValidating the map... ";
    validation = map3->validate();
    switch (validation) {
        case 0:
            cout << "The map is valid." << endl;
            break;
        case 1:
            cout << "The map is not a connected graph" << endl;
            break;
        case 2:
            cout << "At least one continents is not a connected sub-graph" << endl;
            break;
        case 3:
            cout << "At least one territory belongs to more than one continent" << endl;
            break;
        default:
            cout << "Unknown issue!" << endl;
    }
    delete map3;
    map3 = nullptr;

    Map* map4 = MapLoader::loadMapFile("../Map/maps/canada1.map");
    cout << "\n\n\n\nMap 4: Canada - disconnected graph\n" << *map4 << endl;
    cout << "\nValidating the map... ";
    validation = map4->validate();
    switch (validation) {
        case 0:
            cout << "The map is valid." << endl;
            break;
        case 1:
            cout << "The map is not a connected graph" << endl;
            break;
        case 2:
            cout << "At least one continents is not a connected sub-graph" << endl;
            break;
        case 3:
            cout << "At least one territory belongs to more than one continent" << endl;
            break;
        default:
            cout << "Unknown issue!" << endl;
    }
    delete map4;
    map4 = nullptr;


    Map* map5 = MapLoader::loadMapFile("../Map/maps/canada2.map");
    cout << "\n\n\n\nMap 5: Canada - disconnected sub-graph\n" << *map5 << endl;
    cout << "\nValidating the map... ";
    validation = map5->validate();
    switch (validation) {
        case 0:
            cout << "The map is valid." << endl;
            break;
        case 1:
            cout << "The map is not a connected graph" << endl;
            break;
        case 2:
            cout << "At least one continents is not a connected sub-graph" << endl;
            break;
        case 3:
            cout << "At least one territory belongs to more than one continent" << endl;
            break;
        default:
            cout << "Unknown issue!" << endl;
    }
    delete map5;
    map5 = nullptr;
}