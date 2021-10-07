//
// Created by Khoa Trinh on 2021-09-29.
//

#include "GameEngine.h"
#include "../Player/Player.h"
#include <algorithm>
//#include <filesystem>
#include <time.h>
#include <unordered_set>
#include <string>
Phases* phase = new Phases(Phases::START);
std::vector<Player*> GameEngine::players_;
// ----------------------------------START UP----------------------------------------------//
void StartUp::startUp() {
    printTitle();
    while (*phase != Phases::ASSIGNREINFORCEMENT) {
        switch (*phase) {
            case Phases::START:
                loadMap();
                break;
            case Phases::MAPLOADED:
                validateMap();
                break;
            case Phases::MAPVALIDATED:
                createPlayers();
                break;
            case Phases::PLAYERSADDED:
                assignTerritories();
                break;
            default:
                cout << "ERROR" << endl;
                break;
        }
    }
}

void StartUp::loadMap(){
    cout << "***  Inside loadMap  ***" << endl;

    cout << "...loading map" << endl;
    // TBI: load map
    cout << "...map loaded" << endl;

    vector<string> options {"Load a different map", "Validate map"};
    HelperFunctions::printOptions(options);

    HelperFunctions::takeInput(Phases::MAPLOADED);
};

void StartUp::validateMap(){
    cout << "***  Inside validateMap  ***" << endl;
    *phase = Phases::MAPVALIDATED;
};

void StartUp::createPlayers(){
    cout << "***  Inside createPlayers  ***" << endl;
    addPlayers();

    vector<string> options {"Add a different number of players", "Assign territories"};
    HelperFunctions::printOptions(options);

    HelperFunctions::takeInput(Phases::PLAYERSADDED);

};

void StartUp::assignTerritories(){
    cout << "***  Inside assignTerritories  ***" << endl;
    *phase = Phases::ASSIGNREINFORCEMENT;
};

void StartUp::printTitle() {
    cout << ("****************************************") << endl;
    cout << ("*                                      *") << endl;
    cout << ("*               WARZONE                *") << endl;
    cout << ("*                                      *") << endl;
    cout << ("****************************************") << endl;
    cout << ("           Welcome to WARZONE          ") << endl;
}

void StartUp::addPlayers(){
    while (true) {
        cout << "Please enter the number of players: " << endl;
        string input;
        cin >> input;
        if (HelperFunctions::isNumber(input)){

            // TBI: HANDLE ADDING PLAYERS HERE

            cout << input << " players have been added." << endl;
            break;
        }
        else {
            cout << "Invalid input" << endl;
        }
    }
}

// ----------------------------------PLAY----------------------------------------------//

void Play::playGame(){
    printPlayPhaseGreeting();
    while (*phase != Phases::WIN) {
        switch (*phase) {
            case Phases::ASSIGNREINFORCEMENT:
                assignArmies();
                break;
            case Phases::ISSUEORDERS:
                issueOrders();
                break;
            case Phases::EXECUTEORDERS:
                executeOrders();
                break;
            case Phases::WIN:
                cleanUp();
                break;
            default:
                cout << "ERROR" << endl;
                break;
        }
    }
};


void Play::assignArmies(){
    cout << "***  Inside assignArmies  ***" << endl;
    *phase = Phases::ISSUEORDERS;
};

void Play::issueOrders(){
    cout << "***  Inside issueOrders  ***" << endl;

    // TBI: HANDLE ISSUE ORDERS HERE

    vector<string> options {"Issue order", "Complete issue orders"};
    HelperFunctions::printOptions(options);

    HelperFunctions::takeInput(Phases::EXECUTEORDERS);

};

void Play::executeOrders(){
    cout << "***  Inside executeOrders  ***" << endl;

    // TBI: HANDLE EXECUTE ORDERS HERE

    vector<string> options {"Execute orders", "Complete execute orders"};
    HelperFunctions::printOptions(options);

    HelperFunctions::takeInput(Phases::WIN);

};

void Play::cleanUp(){
    cout << "***  Inside cleanUp  ***" << endl;

    // Handle memory leak
    delete phase;
    phase = nullptr;
};

void Play::printPlayPhaseGreeting() {
    cout << ("****************************************") << endl;
    cout << ("*           Setup complete             *") << endl;
    cout << ("*                                      *") << endl;
    cout << ("*             LET'S PLAY               *") << endl;
    cout << ("****************************************") << endl << endl;

}

// --------------------------------HELPER FUNCTIONS-------------------------------------------

void HelperFunctions::printOptions(vector<string> options){
    for (int i = 1; i<= options.size(); i++){
        cout << i << " - " << options[i-1] << endl;
    }
    cout << "Please enter the option number: " << endl;

    // Handle memory leak
    options.clear();

    // This will create a new (temporary) vector and swap its contents with options. The temporary vector is then destroyed, freeing the memory along with it.
    vector<string>().swap(options);
}

bool HelperFunctions::isNumber(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void HelperFunctions::takeInput(Phases nextPhase){
    while (true) {
        string input;
        cin >> input;
        if (input == "1"){
            break;
        }
        else if (input == "2"){
            *phase = nextPhase;
            break;
        }
        else {
            cout << "invalid input" << endl;
            break;
        }
    }
}

// Thong's part


/*
===================================
 Implementation for GameEngine class
===================================
 */

void GameEngine::assignToNeutralPlayer(Territory* territory)
{
    Player* owner = getOwnerOf(territory);
    owner->removeTerritory(territory);

    auto isNeutralPlayer = [](const auto &player) { return player->isNeutral(); };
    auto iterator = find_if(players_.begin(), players_.end(), isNeutralPlayer);
    if (iterator == players_.end())
    {
        Player* neutralPlayer = new Player();
        neutralPlayer->addTerritory(territory);
        players_.push_back(neutralPlayer);
    }
    else
    {
        Player* neutralPlayer = *iterator;
        neutralPlayer->addTerritory(territory);
    }
}

// Find the player who owns the specified territory. Return nullptr if the territory is unowned.
Player* GameEngine::getOwnerOf(Territory* territory)
{
    for (const auto &player : players_)
    {
        std::vector<Territory*> territories = player->getTerritories();
        if (find(territories.begin(), territories.end(), territory) != territories.end())
        {
            return player;
        }
    }
    return nullptr;
}






