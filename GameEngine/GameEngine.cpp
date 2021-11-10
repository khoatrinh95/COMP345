//
// Created by Khoa Trinh on 2021-09-29.
//

#include "../GameEngine/GameEngine.h"
#include "../CommandProcessing/CommandProcessing.h"
#include <algorithm>
#include <math.h>
#include <string>




Deck *GameEngine::deck = new Deck();
Player *GameEngine::neutralPlayer = new Player("Neutral", true);

/*
===================================
 Implementation for GameEngine class
===================================
 */

// Default constructor
GameEngine::GameEngine() : mapDirectory("../Map/maps/") {
    phase = new Phases(Phases::START);
    mode = new Modes(Modes::STARTUP);
    commandProcessor = new FileCommandProcessorAdapter("../GameEngine/GECommands.txt");
    deck->addCard(new Card("bomb"));
    deck->addCard(new Card("reinforcement"));
    deck->addCard(new Card("blockade"));
    deck->addCard(new Card("airlift"));
    deck->addCard(new Card("diplomacy"));
    deck->addCard(new Card("bomb"));
    deck->addCard(new Card("reinforcement"));
    deck->addCard(new Card("blockade"));
    deck->addCard(new Card("airlift"));
    deck->addCard(new Card("diplomacy"));
}

// Copy constructor of Game Engine
GameEngine::GameEngine(const GameEngine &anotherGameEngine) : mapDirectory(anotherGameEngine.mapDirectory) {
    this -> phase = new Phases(*anotherGameEngine.phase);
    this -> mode = new Modes(*anotherGameEngine.mode);
    for (auto player : anotherGameEngine.players_){
        Player* player_temp = new Player(*player);
        this->players_.push_back(player_temp);
    }
    for (auto player : anotherGameEngine.playingOrder){
        Player* player_temp = new Player(*player);
        this->playingOrder.push_back(player_temp);
    }
    this->commandProcessor = new CommandProcessor(*anotherGameEngine.commandProcessor);
    this->deck = new Deck(*anotherGameEngine.deck);
    this->neutralPlayer = new Player(*anotherGameEngine.neutralPlayer);
    this->map_ = new Map(*anotherGameEngine.map_);
}

// Destructor
GameEngine::~GameEngine() {
    // Explicitly call destructor of Player because the players will be dynamically allocated at the beginning of the game
    for (auto player : players_) {
        if(player != nullptr) {
            delete player;
            player = nullptr;
        }
    }
    players_.clear();

    playingOrder.clear();

    // Handle memory leak
    if(phase != nullptr) {
        delete phase;
        phase = nullptr;
    }
    if(mode != nullptr) {
        delete mode;
        mode = nullptr;
    }
    if(commandProcessor != nullptr) {
        delete commandProcessor;
        commandProcessor = nullptr;
    }
    if(deck != nullptr) {
        delete deck;
        deck = nullptr;
    }
    if(neutralPlayer != nullptr) {
        delete neutralPlayer;
        neutralPlayer = nullptr;
    }
    if(map_ != nullptr) {
        delete map_;
        map_ = nullptr;
    }
}

// Assignment operator
GameEngine &GameEngine::operator=(const GameEngine &anotherGameEngine) {
    this -> phase = new Phases(*anotherGameEngine.phase);
    this -> mode = new Modes(*anotherGameEngine.mode);
    for (auto player : anotherGameEngine.players_){
        Player* player_temp = new Player(*player);
        this->players_.push_back(player_temp);
    }
    for (auto player : anotherGameEngine.playingOrder){
        Player* player_temp = new Player(*player);
        this->playingOrder.push_back(player_temp);
    }
    this->commandProcessor = new CommandProcessor(*anotherGameEngine.commandProcessor);
    this->deck = new Deck(*anotherGameEngine.deck);
    this->neutralPlayer = new Player(*anotherGameEngine.neutralPlayer);
    this->map_ = new Map(*anotherGameEngine.map_);
    return *this;
}

// Stream insertion
ostream &operator<<(ostream &stream, const GameEngine &gameEngine) {
//    string toPrint = "Here are the players currently playing: \n";
//    for (auto player : gameEngine.players_){
//        toPrint.append(player->getName()).append("\n");
//    }
//    stream << toPrint;
    stream << "Here are the players currently playing:\n";
    for (auto player : gameEngine.players_) {
        if(player != nullptr) {
            stream << *player << "\n";
        }
    }
    return stream;
}

void GameEngine::assignToNeutralPlayer(Territory* territory)
{
    Player* owner = territory->getOwner();
    owner->removeTerritory(territory);
    territory->setOwner(neutralPlayer);
    neutralPlayer->addTerritory(territory);
}

const vector<Player *> &GameEngine::getPlayers() {
    return players_;
}

void GameEngine::addPlayersToList(Player* player) {
    players_.push_back(player);
}

/**
 * remove player from a player list
 * @param player
 */
void GameEngine::removePlayer(Player *player) {
    for (int i = 0; i<players_.size();i++){
        if (players_.at(i)->getName()==player->getName()){
            players_.erase(next(begin(players_), + i));
            delete player;
            player = nullptr;
        }
    }
}

/**
 * Generate textual list of players
 */
string GameEngine::getPlayersNames() const {
    string names;
    int count(0);
    for(auto &player : players_) {
        names += to_string(++count) + ": " + player->getName() + "\n";
    }
    return names;
}

/**
 * Generate textual list of players according to playing order
 */
string GameEngine::getPlayingOrderPlayersNames() const {
    string names;
    int count(0);
    for(auto &player : playingOrder) {
        names += to_string(++count) + ": " + player->getName() + "\n";
    }
    return names;
}




// ----------------------------------Startup----------------------------------------------//
/**
 * Game startup phase
 */
void GameEngine::startupPhase() {
    printTitle();

    // get commands until getting to the playing mode
    while (*phase != Phases::ASSIGNREINFORCEMENT) {
        Command *command = commandProcessor->getCommand();
        commandProcessor->validate(command, phase);
        string instruction = command->getInstruction();
        cout << "Processing command \"" << command->getCommand() << "\"... " << endl;

        // verify if a command is valid
        if(!instruction.empty()) {

            // perform the command's required action then save the effect and go to the next phase
            if(instruction == "loadmap" && (*phase == Phases::START || *phase == Phases::MAPLOADED)) {

                // loading map
                cout << "Loading map \"" << command->getArgument() << "\"... " << endl;
                loadMap(command->getArgument());
                command->saveEffect("Map [" + command->getArgument() + "] loaded. Transition to [maploaded]");
                transition(Phases::MAPLOADED);
//                *phase = Phases::MAPLOADED;
            } else if(instruction == "validatemap" && *phase == Phases::MAPLOADED) {

                // validating map and printing the result
                cout << "Validating the map... " << endl;
                int validation = map_->validate();
                switch (validation) {
                    case 0:
                        cout << "The map is valid." << endl;
                        command->saveEffect("Map validated. Transition to [mapvalidated]");
                        transition(Phases::MAPVALIDATED);
//                        *phase = Phases::MAPVALIDATED;
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
            } else if(instruction == "addplayer" && (*phase == Phases::MAPVALIDATED || *phase == Phases::PLAYERSADDED) && players_.size() < 6) {

                // adding a player
                cout << "Adding player \"" << command->getArgument() << "\"... " << endl;
                players_.push_back(new Player(command->getArgument()));
                cout << "Players joined the game:" << endl;
                cout << getPlayersNames() << endl;
                command->saveEffect("Player [" + command->getArgument() + "] added. Transition to [playersadded]");
                transition(Phases::PLAYERSADDED);
//                *phase = Phases::PLAYERSADDED;
            } else if(instruction == "gamestart" && *phase == Phases::PLAYERSADDED && players_.size() >= 2) {

                // initializing the game
                cout << "Starting the game... " << endl;
                assignTerritories();
                cout << "Territories assigned" << endl;
                assignPlayingOrder();
                cout << "Playing order determined" << endl;
                cout << "Order of play of the players:" <<endl;
                cout << getPlayingOrderPlayersNames() << endl;
                initialReinforcement();
                cout << "Initial reinforcement accomplished" << endl;
                initialCardDrawing();
                cout << "Initial cards drawn" << endl;
                command->saveEffect("Game initiated. Territories distributed. Playing order determined. Initial reinforcement accomplished. Initial cards drawn. Transition to [assignreinforcement]");
                transition(Phases::ASSIGNREINFORCEMENT);
//                *phase = Phases::ASSIGNREINFORCEMENT;
                *mode = Modes::STARTUP;
            } else {
                command->saveEffect("Wrong command. Command Ignored.");
                cout << "ERROR!" << endl;
            }
        } else {
            command->saveEffect("Invalid command. Command Ignored.");
            cout << "Invalid command!" << endl;
        }
    }
    cout << "Game initialization done!" << endl;
    printPlayPhaseGreeting();
}



void GameEngine::loadMap(string filename){
    cout << "Loading map ..." << endl;
    map_ = MapLoader::loadMapFile(mapDirectory + filename);
    cout << "Map was loaded successfully!" << endl;
};

void GameEngine::validateMap(Phases *phase){
    cout << "***  Inside validateMap  ***" << endl;
    transition(Phases::MAPVALIDATED);
//    *phase = Phases::MAPVALIDATED;
};


/**
 * Assign territories to players
 */
void GameEngine::assignTerritories() {
    cout << "***  Inside assignTerritories  ***" << endl;
    int numPlayers = static_cast<int>(players_.size());
    Territory **territories = map_->getTerritories();
    vector<Territory*> vecTerritories;

    // sorting territories descendingly according to number of bonus of their containing continent
    for (int i = 0; i < map_->getNumTerritories(); i++) {
        vecTerritories.push_back(territories[i]);
    }
    sort(vecTerritories.begin(), vecTerritories.end(), [](Territory *a, Territory *b) { return a->getContinent()->getBonus() > b->getContinent()->getBonus(); });

    // assigning the sorted territories to players repeatedly from 1 to N then from N to 1 until all territories are assigned
    for (int i = 0; i < map_->getNumTerritories(); i++) {
        int j = i % (2 * numPlayers);
        int k;
        if(j < numPlayers) {
            k = j;
        } else {
            k = (2 * numPlayers) - j - 1;
        }
        vecTerritories.at(i)->setOwner(players_.at(k));
        players_.at(k)->addTerritory(vecTerritories.at(i));
    }
};

/**
 * Determine the playing order
 */
void GameEngine::assignPlayingOrder() {
    // the playing order is randomly determined from the set of all players
    srand(static_cast<unsigned>(time(nullptr)));
    int numAssignedPlayers = 0;
    int numPlayers = static_cast<int>(players_.size());
    int alreadyAssignedPlayers[numPlayers];
    for (int i = 0; i < numPlayers; i++) {
        alreadyAssignedPlayers[i] = 0;
    }
    while (numAssignedPlayers < numPlayers) {
        int playerIndex;
        do {
            playerIndex = rand() % numPlayers;
        }
        while (alreadyAssignedPlayers[playerIndex] != 0);
        alreadyAssignedPlayers[playerIndex]++;
        playingOrder.push_back(players_.at(playerIndex));
        numAssignedPlayers++;
    }
}

/**
 * Initial reinforcement
 */
void GameEngine::initialReinforcement() {
    for (auto &player : players_) {
        player->setReinforcementPool(50);
    }
}

/**
 * Each player draws 2 cards
 */
void GameEngine::initialCardDrawing() {
    for (auto &player : players_) {
        for (int i = 0; i < 2; i++) {
            if(player == nullptr) {
                if(player->getPlayerCards() == nullptr && deck == nullptr) {
                    player->getPlayerCards()->drawFromDeck(deck);
                }
            }
        }
    }
}


void GameEngine::printTitle() {
    cout << ("****************************************") << endl;
    cout << ("*                                      *") << endl;
    cout << ("*               WARZONE                *") << endl;
    cout << ("*                                      *") << endl;
    cout << ("****************************************") << endl;
    cout << ("           Welcome to WARZONE          ") << endl;
}

void GameEngine::printPlayPhaseGreeting() {
    cout << ("****************************************") << endl;
    cout << ("*           Setup complete             *") << endl;
    cout << ("*                                      *") << endl;
    cout << ("*             LET'S PLAY               *") << endl;
    cout << ("****************************************") << endl << endl;
}


// ----------------------------------PLAY----------------------------------------------//

////////////////////////////////////////////////////////////SARAH _PART 3

void GameEngine::mainGameLoop() {
    bool ownAllContinents = false;
    while (players_.size()!=1) {
        // remove a player from the game if s/he does not own any territory

        // add armies to each player Reinforcement Pool
        reinforcementPhase();

        // let each player decide his/her order list
        issueOrdersPhase();

        // execute each player orders from his/her order list
        executeOrdersPhase();



        for (auto &player :players_){
            if(player->getTerritories().size() == 0){
                removePlayer(player);
            }
        }
    }
    cout << "The winner of the game is : "<< players_.at(0)->getName()<< endl;
}

/**
 * assign reinforcement for each player
 */
void GameEngine::reinforcementPhase() {
    for (auto &player:players_){
        int armies = floor(player->getTerritories().size()/3);
        for(int i = 0; i<map_->getNumContinent();i++){
            if (map_->getContinent()[i]->getOwner()==player){
                armies = armies + map_->getContinent()[i]->getBonus();
            }
        }
        if (armies<3){
            armies = 3 ;
        }
        player->setReinforcementPool(armies);
    }
}

/**
 * asking the player to start their issuing their orders
 */
void GameEngine::issueOrdersPhase() {
    for (auto &player : players_){
        player->issueOrder();
    }
}

/**
 * execute player orders from their order list
 */
void GameEngine::executeOrdersPhase() {
    int longestOrderList = 0 ;
    // find the longest order list of a player
    for (auto &player:players_){
        if (player->getPlayerOrdersList()->size()>longestOrderList){
            longestOrderList = player->getPlayerOrdersList()->size();
        }
    }
    // execute all deploy orders of all players in round-robin fashion
    for (int i = 0 ; i<longestOrderList; i++){
        for (auto &player:players_){
            if(player->getPlayerOrdersList()->size()>i){
                if (player->getPlayerOrdersList()->getOrders().at(i)->getType() == DEPLOY){
                    player->getPlayerOrdersList()->getOrders().at(i)->execute();
                }
            }
        }
    }
    // execute all non-deploy orders of all prayers in round-robin fashion
    for (int i = 0 ; i<longestOrderList; i++){
        for (auto &player:players_){
            if(player->getPlayerOrdersList()->size()>i){
                if (player->getPlayerOrdersList()->getOrders().at(i)->getType() != DEPLOY){
                    player->getPlayerOrdersList()->getOrders().at(i)->execute();
                }
            }
        }
    }

}

// Iloggable
string GameEngine::stringToLog() {
    return "Game Engine - changing to phase: ";
}

string GameEngine::phaseToString(Phases *phase) {
    switch (*phase){
        case Phases::ASSIGNREINFORCEMENT:
            return "Assign Reinforcement";
        case Phases::MAPVALIDATED:
            return "Map Validated";
        case Phases::PLAYERSADDED:
            return "Players Added";
        case Phases::MAPLOADED:
            return "Map loaded";
        case Phases::EXECUTEORDERS:
            return "Execute Orders";
        case Phases::ISSUEORDERS:
            return "Issue Orders";
        case Phases::WIN:
            return "Win";
        case Phases::START:
            return "Start";
    }
}

void GameEngine::transition(Phases phaseToTransition) {
    *phase = phaseToTransition;
    string phaseString = phaseToString(phase);
    contentToLog = phaseString;
    notify();
}

