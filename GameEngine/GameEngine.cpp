//
// Created by Khoa Trinh on 2021-09-29.
//

#include "GameEngine.h"
#include <algorithm>

#include <string>

std::vector<Player*> GameEngine::players_;
// ----------------------------------START UP----------------------------------------------//
void StartUp::startUp(Phases *phase) {
    printTitle();
    while (*phase != Phases::ASSIGNREINFORCEMENT) {
        switch (*phase) {
            case Phases::START:
                loadMap(phase);
                break;
            case Phases::MAPLOADED:
                validateMap(phase);
                break;
            case Phases::MAPVALIDATED:
                GameEngine::clearPlayerList();
                createPlayers(phase);
                break;
            case Phases::PLAYERSADDED:
                assignTerritories(phase);
                break;
            default:
                cout << "ERROR" << endl;
                break;
        }
    }
}

void StartUp::loadMap(Phases *phase){
    cout << "***  Inside loadMap  ***" << endl;

    cout << "...loading map" << endl;
    // TBI: load map
    cout << "...map loaded" << endl;

    vector<string> options {"Load a different map", "Validate map"};
    HelperFunctions::printOptions(options);

    HelperFunctions::takeInput(phase, Phases::MAPLOADED);
};

void StartUp::validateMap(Phases *phase){
    cout << "***  Inside validateMap  ***" << endl;
    *phase = Phases::MAPVALIDATED;
};

void StartUp::createPlayers(Phases *phase){
    cout << "***  Inside createPlayers  ***" << endl;
    addPlayers();

    vector<string> options {"Add a different number of players", "Assign territories"};
    HelperFunctions::printOptions(options);

    HelperFunctions::takeInput(phase, Phases::PLAYERSADDED);

};

void StartUp::assignTerritories(Phases *phase){
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
            int numOfPlayers = stoi(input);
            for (int i = 0; i < numOfPlayers; i++){
                cout << "Please enter the name of player " << (i+1) << endl;
                string name;
                cin >> name;
                Player *player = new Player(name);
                GameEngine::addPlayersToList(player);
            }
            cout << numOfPlayers << " players have been added." << endl;
            break;
        }
        else {
            cout << "Invalid input" << endl;
        }
    }
}

// ----------------------------------PLAY----------------------------------------------//

void Play::playGame(Phases *phase){
    printPlayPhaseGreeting();
    while (*phase != Phases::WIN) {
        switch (*phase) {
            case Phases::ASSIGNREINFORCEMENT:
                assignArmies(phase);
                break;
            case Phases::ISSUEORDERS:
                issueOrders(phase);
                break;
            case Phases::EXECUTEORDERS:
                executeOrders(phase);
                break;
            case Phases::WIN:
//                cleanUp();
                break;
            default:
                cout << "ERROR" << endl;
                break;
        }
    }
};


void Play::assignArmies(Phases *phase){
    cout << "***  Inside assignArmies  ***" << endl;
    *phase = Phases::ISSUEORDERS;
};

void Play::issueOrders(Phases *phase){
    cout << "***  Inside issueOrders  ***" << endl;

    // TBI: HANDLE ISSUE ORDERS HERE

    vector<string> options {"Issue order", "Complete issue orders"};
    HelperFunctions::printOptions(options);

    HelperFunctions::takeInput(phase, Phases::EXECUTEORDERS);

};

void Play::executeOrders(Phases *phase){
    cout << "***  Inside executeOrders  ***" << endl;

    // TBI: HANDLE EXECUTE ORDERS HERE

    vector<string> options {"Execute orders", "Complete execute orders"};
    HelperFunctions::printOptions(options);

    HelperFunctions::takeInput(phase, Phases::WIN);

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

void HelperFunctions::takeInput(Phases *phase, Phases nextPhase){
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

// Default constructor
GameEngine::GameEngine(){
    phase = new Phases(Phases::START);
}

// Copy constructor of Game Engine
GameEngine::GameEngine(const GameEngine &anotherGameEngine) {
    this -> phase = new Phases(*anotherGameEngine.phase);
    for (auto player : anotherGameEngine.players_){
        Player* player_temp = new Player(*player);
        this->players_.push_back(player_temp);
    }
}

// Destructor
GameEngine::~GameEngine() {
    cout << "***  Inside Game Engine destructor  ***" << endl;
    // Explicitly call destructor of Player because the players will be dynamically allocated at the beginning of the game
    for (auto player : players_){
        delete player;
    }
    players_.clear();

    // Handle memory leak
    delete phase;
    phase = nullptr;
}

// Assignment operator
GameEngine &GameEngine::operator=(const GameEngine &anotherGameEngine) {
    for (auto player : anotherGameEngine.players_){
        Player* player_temp = new Player(*player);
        this->players_.push_back(player_temp);
    }
    return *this;
}

// Stream insertion
ostream &operator<<(ostream &stream, const GameEngine &gameEngine) {
    string toPrint = "Here are the players currently playing: \n";
    for (auto player : gameEngine.players_){
        toPrint.append(player->getName()).append("\n");
    }
    stream << toPrint;
    return stream;
}

void GameEngine::playGame() {
    StartUp::startUp(phase);
    Play::playGame(phase);
}

const vector<Player *> &GameEngine::getPlayers() {
    return players_;
}

void GameEngine::addPlayersToList(Player* player) {
    players_.push_back(player);
}

void GameEngine::clearPlayerList() {
    for (auto player : players_){
        delete player;
        player = NULL;
    }
    players_.clear();
}





