//
// Created by Khoa Trinh on 2021-09-29.
//

#include "../GameEngine/GameEngine.h"
#include "../CommandProcessing/CommandProcessing.h"
#include <algorithm>
#include <math.h>
#include <string>
#include "../Cards/Cards.h"
#include "../PlayerStrategy/PlayerStrategy.h"




Deck *GameEngine::deck = new Deck();
Player *GameEngine::neutralPlayer = new Player("Neutral", true);
vector<PlayerStrategy*>GameEngine:: strategyType = {new AggressivePlayerStrategy(),new HumanPlayerStrategy(),
                                                    new NeutralPlayerStrategy(), new CheaterPlayerStrategy(),
                                                    new BenevolentPlayerStrategy()
};

/*
===================================
 Implementation for GameEngine class
===================================
 */

// Default constructor
GameEngine::GameEngine() : MAP_DIRECTORY("../Map/maps/"), MIN_NUM_PLAYERS(2), MAX_NUM_PLAYERS(6) {
    map_ = nullptr;
    phase = new Phases(Phases::START);
    mode = new Modes(Modes::STARTUP);
    commandProcessor = new FileCommandProcessorAdapter("../GameEngine/GECommands.txt");
    for(int i = 0; i < 10; i++) {
        deck->addCard(new Card("bomb"));
        deck->addCard(new Card("reinforcement"));
        deck->addCard(new Card("blockade"));
        deck->addCard(new Card("airlift"));
        deck->addCard(new Card("diplomacy"));
    }
}

// Copy constructor of Game Engine
GameEngine::GameEngine(const GameEngine &anotherGameEngine) : MAP_DIRECTORY(anotherGameEngine.MAP_DIRECTORY), MIN_NUM_PLAYERS(anotherGameEngine.MIN_NUM_PLAYERS), MAX_NUM_PLAYERS(anotherGameEngine.MAX_NUM_PLAYERS) {
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
    for (auto &player : players_) {
        if(player != nullptr) {
            if(player->getPlayerCards() != nullptr) {
                player->getPlayerCards()->removeAllCards();
            }
            delete player;
            player = nullptr;
        }
    }
    players_.clear();
    playingOrder.clear();

    // delete strategies
    for(auto &strategy : strategyType) {
        if(strategy != nullptr) {
            delete strategy;
            strategy = nullptr;
        }
    }

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
        this->players_.push_back(new Player(*player));
    }
    for (auto player : anotherGameEngine.playingOrder){
        this->playingOrder.push_back(new Player(*player));
    }
    this->commandProcessor = new CommandProcessor(*anotherGameEngine.commandProcessor);
    this->deck = new Deck(*anotherGameEngine.deck);
    this->neutralPlayer = new Player(*anotherGameEngine.neutralPlayer);
    this->map_ = new Map(*anotherGameEngine.map_);
    return *this;
}

// Stream insertion
ostream &operator<<(ostream &stream, const GameEngine &gameEngine) {
    stream << "Here are the players currently playing:" << endl;
    for (auto player : gameEngine.players_) {
        if(player != nullptr) {
            stream << *player << endl;
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
    for (int i = 0; i<playingOrder.size();i++){
        if (playingOrder.at(i)->getName()==player->getName()){
            cout << "***\tRemoving "<< playingOrder.at(i)->getName() << " from the game"<<endl;
            player->getPlayerCards()->removeAllCards();
            playingOrder.erase(next(begin(playingOrder), + i));
            break;
        }
    }
    OrdersList* ol = player -> getPlayerOrdersList();
    int size = ol->size();
    for (int k =0 ; k<size; k++){
        ol->popTopOrder();
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
    cout << "The game is currently in state: " << phaseToString(*phase) << endl;
    cout << "The game is currently in mode: " << modeToString(*mode) << endl;

    // get commands until getting to the playing mode
    string instruction;
    Command *command = nullptr;
    while (*mode == Modes::STARTUP && instruction != "eof") {
        command = commandProcessor->getCommand();
        commandProcessor->validate(command, phase);
        instruction = command->getInstruction();
        cout << "Processing command \"" << command->getCommand() << "\"... " << endl;

        // verify if a command is valid
        if(!instruction.empty()) {

            // perform the command's required action then save the effect and go to the next phase
            if(instruction == "tournament" && command->getMapList().size() >= 1 && command->getMapList().size() <= 5 && command->getplayerStrategiesList().size() >= 2  && command->getplayerStrategiesList().size() <= 4 && command->getNumOfGames() >= 1 && command->getNumOfGames() <= 5 && command->getNumOfTurns() >= 10 && command->getNumOfTurns() <= 50 && *phase == Phases::START) {
                // playing in tournament
                cout << "Playing a tournament..." << endl;
                int numberOfGames = command->getNumOfGames(); // number of games in a tournament
                int numberOfMaxTurns = command->getNumOfTurns(); // number of max turns in a game
                vector<string> mapsList = command->getMapList(); // list of maps
                int numberOfMaps = mapsList.size(); // number of maps

                // add players
                vector<string> playersList = command->getplayerStrategiesList();
                for(int i = 0; i < playersList.size(); i++) {
                    players_.push_back(new Player("Player" + to_string(i+1) + "_" + playersList.at(i),Player::parsePlayerStrategy(playersList.at(i))));
                }

                // prepare report of the results
                string tournamentResult;
                const int MAX_TABLE_CELL_LENGTH = 25;
                tournamentResult.append(MAX_TABLE_CELL_LENGTH, ' ');
                for(int i = 0; i < numberOfGames; i++) {
                    string gameNumberText = "Game " + to_string(i + 1);
                    tournamentResult += " | " + gameNumberText.append(MAX_TABLE_CELL_LENGTH - gameNumberText.length(), ' ');
                }
                string tournamentMapResult[numberOfMaps];

                // for each map, load map, validate map then start games
                for (int i = 0; i < numberOfMaps; i++) {
                    cout << "loading map: " << mapsList.at(i) << endl;
                    startupMapLoading(mapsList.at(i));
                    string mapNumberText = "Map " + to_string(i + 1);
                    tournamentMapResult[i] = mapNumberText.append(MAX_TABLE_CELL_LENGTH - mapNumberText.length(), ' ');
                    string validationResult = startupMapValidation();
                    if(validationResult != "Map validated. Transition to [mapvalidated]") {
                        break;
                    }
                    // start games
                    for(int j = 0; j < numberOfGames; j++) {
                        cout << "*************************\n*\tGame " << j+1 << "\t\t*\n*************************\n" << endl;
                        startupGameInitialization(); // initialize game
                        string result = tournamentPlay(numberOfMaxTurns);
                        cout << "Result: " << result << endl;
                        tournamentMapResult[i] += " | " + result.append(MAX_TABLE_CELL_LENGTH - result.length(), ' ');


                        // clear playing order list
                        playingOrder.clear();

                        // return all cards in a player's hand to the deck, and remove all territories
                        for (auto &player : players_) {
                            if(player != nullptr) {
                                if(player->getPlayerCards() != nullptr) {
                                    player->getPlayerCards()->removeAllCards();
                                }
                                // clear all player's ordersList
                                OrdersList* ol = player -> getPlayerOrdersList();
                                int size = ol->size();
                                for (int k =0 ; k<size; k++){
                                    ol->popTopOrder();
                                }
                                player->removeAllTerritories();
                            }
                        }
                        resetPlayerStrategy();
                    }
                    tournamentResult += "\n" + tournamentMapResult[i];
                }
                cout << "Tournament Result:\n" << tournamentResult << endl;
                command->saveEffect("Tournament played.");
                contentToLog = "Game Engine - tournament result:\n" + tournamentResult;
                notify();

            } else if(instruction == "loadmap" && (*phase == Phases::START || *phase == Phases::MAPLOADED)) {

                // loading map
                cout << "Loading map \"" << command->getArgument() << "\"... " << endl;
                loadMap(command->getArgument());
                cout << "The loaded map is described as follows:" << endl << *map_ << endl;
                command->saveEffect("Map [" + command->getArgument() + "] loaded. Transition to [maploaded]");
                transition(Phases::MAPLOADED);
                cout << "The game is currently in state: " << phaseToString(*phase) << endl;
            } else if(instruction == "validatemap" && *phase == Phases::MAPLOADED) {

                // validating map and printing the result
                cout << "Validating the map... " << endl;
                int validation = map_->validate();
                switch (validation) {
                    case 0:
                        cout << "The map is valid." << endl;
                        command->saveEffect("Map validated. Transition to [mapvalidated]");
                        transition(Phases::MAPVALIDATED);
                        cout << "The game is currently in state: " << phaseToString(*phase) << endl;
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
                if (validation > 0) {
                    command->saveEffect("Map validation failed");
                }
            } else if(instruction == "addplayer" && (*phase == Phases::MAPVALIDATED || *phase == Phases::PLAYERSADDED) && players_.size() < MAX_NUM_PLAYERS) {

                // adding a player
                cout << "Adding player \"" << command->getArgument() << "\"... " << endl;
                players_.push_back(new Player(command->getArgument()));
                cout << "Players joined the game:" << endl;
                cout << getPlayersNames() << endl;
                command->saveEffect("Player [" + command->getArgument() + "] added. Transition to [playersadded]");
                transition(Phases::PLAYERSADDED);
                cout << "The game is currently in state: " << phaseToString(*phase) << endl;
            } else if(instruction == "gamestart" && *phase == Phases::PLAYERSADDED && players_.size() >= MIN_NUM_PLAYERS) {

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
                cout << "The game is currently in state: " << phaseToString(*phase) << endl;
                *mode = Modes::PLAY;
                cout << "The game is currently in mode: " << modeToString(*mode) << endl;
            } else {
                command->saveEffect("Wrong command. Command Ignored.");
                cout << "ERROR!" << endl;
            }
        } else {
            command->saveEffect("Invalid command. Command Ignored.");
            cout << "Invalid command!" << endl;
        }
    }
    if(*phase == Phases::ASSIGNREINFORCEMENT) {
        cout << "Game initialization done!" << endl;
        printPlayPhaseGreeting();
    }
}

string GameEngine::startupMapLoading(string map) {
    // delete map
    if(map_ != nullptr) {
        delete map_;
        map_ = nullptr;
    }

    // loading map
    cout << "Loading map \"" << map << "\"... " << endl;
    loadMap(map);
    cout << "The loaded map is described as follows:" << endl << *map_ << endl;
    transition(Phases::MAPLOADED);
    cout << "The game is currently in state: " << phaseToString(*phase) << endl;
    return "Map [" + map + "] loaded. Transition to [maploaded]";
}

string GameEngine::startupMapValidation() {
    // validating map and printing the result
    cout << "Validating the map... " << endl;
    int validation = map_->validate();
    switch (validation) {
        case 0:
            cout << "The map is valid." << endl;
            transition(Phases::MAPVALIDATED);
            cout << "The game is currently in state: " << phaseToString(*phase) << endl;
            return "Map validated. Transition to [mapvalidated]";
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
    return "Map validation failed.";
}

string GameEngine::startupGameInitialization() {
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
    transition(Phases::ASSIGNREINFORCEMENT);
    cout << "The game is currently in state: " << phaseToString(*phase) << endl;
    *mode = Modes::PLAY;
    cout << "The game is currently in mode: " << modeToString(*mode) << endl;
    return "Game initiated. Territories distributed. Playing order determined. Initial reinforcement accomplished. Initial cards drawn. Transition to [assignreinforcement]";
}


string GameEngine::tournamentPlay(int numberOfMaxTurns) {
    int turnCount = 0;
    while (playingOrder.size() > 1 && turnCount < numberOfMaxTurns) {
        printPlayerStrategy();

        turnCount++;
        // add armies to each player Reinforcement Pool
        cout << "***********************************"<<endl;
        cout << "**\t REINFORCEMENT PHASE\t**"<<endl;
        cout << "***********************************"<<endl;

        transition(Phases::ASSIGNREINFORCEMENT);
        reinforcementPhase();

        // let each player decide his/her order list
        cout << "***********************************"<<endl;
        cout << "**\t ISSUE ORDER PHASE\t**"<<endl;
        cout << "***********************************"<<endl;

        transition(Phases::ISSUEORDERS);
        issueOrdersPhase();
        cout << endl;

        // execute each player orders from his/her order list

        cout << "***********************************"<<endl;
        cout << "**\t EXECUTE ORDER PHASE\t**"<<endl;
        cout << "***********************************"<<endl;

        transition(Phases::EXECUTEORDERS);
        executeOrdersPhase();
        cout << endl;
    }
    transition(Phases::WIN);
    if(playingOrder.size() == 1) {
        cout << "The winner of the game is : "<< playingOrder.at(0)->getName()<<" ownes ";
        cout <<playingOrder.at(0)->getTerritories().size()<<" territories"<<endl;
        return playingOrder.at(0)->getName();
    }
    cout << "The game ended in draw" << endl;
    return "Draw";
}



/**
 * Loads the map form a file
 * @param filename
 */
void GameEngine::loadMap(string filename){
    cout << "Loading map ..." << endl;
    map_ = MapLoader::loadMapFile(MAP_DIRECTORY + filename);
    cout << "Map was loaded successfully!" << endl;
};

/**
 * validates the loaded map
 * @param phase
 */
void GameEngine::validateMap(Phases *phase){
    cout << "***  Inside validateMap  ***" << endl;
    transition(Phases::MAPVALIDATED);
};


/**
 * Assign territories to players
 */
void GameEngine::assignTerritories() {
    int numPlayers = static_cast<int>(players_.size());
    Territory **territories = map_->getTerritories();
    vector<Territory*> vecTerritories;

    // sorting territories descendingly according to number of bonus of their containing continent
    if(territories != nullptr) {
        for (int i = 0; i < map_->getNumTerritories(); i++) {
            vecTerritories.push_back(territories[i]);
        }
    }
    sort(vecTerritories.begin(), vecTerritories.end(), [](Territory *a, Territory *b) { return a->getContinent()->getBonus() > b->getContinent()->getBonus(); });

    /////////////////////////////// for demo purpose only
    cout << "\n================  Territories ordered according to amount of continent's bonus  =====================" << endl;
    for(auto& ter : vecTerritories) {
        cout << ter->getName() << " in " << ter->getContinent()->getName() << " = " << ter->getContinent()->getBonus() << endl;
    }
    cout << "\n==============  Assignment of territories to players ================" << endl;
    ///////////////////////////////////////////

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

        /////////////////////////////// for demo purpose only
        cout << vecTerritories.at(i)->getName() << " assigned to " << players_.at(k)->getName() << endl;
        //////////////////////////////////////////////
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
    playingOrder.clear();
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
            if(player != nullptr) {
                if(player->getPlayerCards() != nullptr && deck != nullptr) {
                    player->getPlayerCards()->drawFromDeck(deck);
                }
            }
        }
    }
}


void GameEngine::printTitle() {
    cout << ("\n****************************************") << endl;
    cout << ("*                                      *") << endl;
    cout << ("*               WARZONE                *") << endl;
    cout << ("*                                      *") << endl;
    cout << ("****************************************") << endl;
    cout << ("           Welcome to WARZONE          ") << endl;
}

void GameEngine::printPlayPhaseGreeting() {
    cout << ("\n****************************************") << endl;
    cout << ("*           Setup complete             *") << endl;
    cout << ("*                                      *") << endl;
    cout << ("*             LET'S PLAY               *") << endl;
    cout << ("****************************************") << endl << endl;
}

void GameEngine::printPlayerStrategy(){
    cout<<"STRATEGY: " << endl;
    for (auto &player : playingOrder){
        player->getStrategy()->print(player);
    }
    cout << endl << endl;
}

// ----------------------------------PLAY----------------------------------------------//

/**
 * Main game loop
 */
void GameEngine::mainGameLoop() {
    ////////////////////////////////////////////////////
//    playingOrder.at(0)->setStrategy(strategy::Benevolent);
//    playingOrder.at(1)->setStrategy(strategy::Neutral);
////    playingOrder.at(2)->setStrategy(strategy::Benevolent);


    ////////////////////////////////////////////////
    while (playingOrder.size()!=1) {
//        for (auto&player:playingOrder){
//            player->getStrategy()->print(player);
//        }
//int n = 5;
//    while (n >1){
        // add armies to each player Reinforcement Pool
        cout << "***********************************"<<endl;
        cout << "**\t REINFORCEMENT PHASE\t**"<<endl;
        cout << "***********************************"<<endl;

        transition(Phases::ASSIGNREINFORCEMENT);
        reinforcementPhase();

        // let each player decide his/her order list
    cout << "***********************************"<<endl;
    cout << "**\t ISSUE ORDER PHASE\t**"<<endl;
    cout << "***********************************"<<endl;

        transition(Phases::ISSUEORDERS);
        issueOrdersPhase();
        cout << endl;

        // execute each player orders from his/her order list

    cout << "***********************************"<<endl;
    cout << "**\t EXECUTE ORDER PHASE\t**"<<endl;
    cout << "***********************************"<<endl;

        transition(Phases::EXECUTEORDERS);
        executeOrdersPhase();
        cout << endl;
    printPlayerStrategy();
    }
    transition(Phases::WIN);
    cout << "The winner of the game is : "<< playingOrder.at(0)->getName()<<" ownes ";
    cout <<playingOrder.at(0)->getTerritories().size()<<" territories"<<endl;
    playingOrder.at(0)->getPlayerCards()->removeAllCards();

    OrdersList* ol = playingOrder.at(0) -> getPlayerOrdersList();
    int size = ol->size();
    for (int k =0 ; k<size; k++){
        ol->popTopOrder();
    }
}

/**
 * assign reinforcement for each player
 */
void GameEngine::reinforcementPhase() {
    for (auto &player:playingOrder){
        int armies = floor(double(player->getTerritories().size())/3);
        cout << "Since "<< player->getName() << " owns "<< player->getTerritories().size()<< " territories, "<< armies;
        cout<<" armies will be added to his reinforcement pool"<< endl;
        for(int i = 0; i<map_->getNumContinent();i++){
            if (map_->getContinent()[i]->getOwner()==player){
                cout <<"Adding "<<map_->getContinent()[i]->getName()<<" bonus armies to "<<player->getName()<<" to his reinforcement pool"<< endl;
                armies = armies + map_->getContinent()[i]->getBonus();
            }
        }
        if (armies<3){
            cout << "However,the number of armies calculated to "<<player->getName()<<" is less than 3; therefore, the player "
                                                                              "will be given 3 armies instead"<< endl;
            armies = 3 ;
        }
        player->assignReinforcementToPlayer(armies);
        cout<< player->getName() << " has new "<< player->getReinforcementPool() << " armies in his reinforcement pool" << endl;
        cout<<endl;
    }
}

/**
 * asking the player to start their issuing their orders
 */
void GameEngine::issueOrdersPhase() {
    for (auto &player : playingOrder){
        cout << "***\t\tIt is "<<player->getName() << " turn to issue Orders\t\t***"<<endl;
        player->issueOrder();
        cout<<endl;
    }
}

/**
 * execute player orders from their order list
 */
void GameEngine::executeOrdersPhase() {
    int longestOrderList = 0 ;
    int sum = 0 ;
    // find the longest order list of a player
    for (auto &player:playingOrder){
        sum = sum + player->getPlayerOrdersList()->size();
        if (player->getPlayerOrdersList()->size()>longestOrderList){
            longestOrderList = player->getPlayerOrdersList()->size();
        }
    }

    bool reach_end = false;
    while(!reach_end) {
        for (auto &player: playingOrder) {
            int i = 0;
            for (; i < longestOrderList; i++) {
                if (i < player->getPlayerOrdersList()->size() &&
                    player->getPlayerOrdersList()->getOrders().at(i)->getType() == DEPLOY) {
                    cout << "The execution for the order " << *player->getPlayerOrdersList()->getOrders().at(i)
                         << " of " << player->getName() << endl;
                    Order *order = player->getPlayerOrdersList()->getOrders().at(i);
                    order->execute();
                    player->getPlayerOrdersList()->removeOrder(order);
                    sum = sum-1;
                    break;
                }
            }
            if (player->getPlayerOrdersList()->size() == 0 || (i>=player->getPlayerOrdersList()->size() && player == playingOrder.at(playingOrder.size()-1))){
                reach_end = true;
            }
        }
    }
    bool onePlayer  = false;
    while (sum>0 && !onePlayer){
        for (auto &player: playingOrder) {
            int i = 0;
            for (; i < longestOrderList; i++) {
                if (i < player->getPlayerOrdersList()->size()) {
                    cout << "The execution for the order " << *player->getPlayerOrdersList()->getOrders().at(i)
                         << " of ";
                    cout << player->getName() << endl;
                    Order *order = player->getPlayerOrdersList()->getOrders().at(i);
                    order->execute();
                    player->getPlayerOrdersList()->removeOrder(order);
                    sum = sum - 1;
                    break;
                }
            }
            for (auto &player :playingOrder){
                if(player->getTerritories().size() == 0){
                    sum =sum-player->getPlayerOrdersList()->size();
                    removePlayer(player);
                    break;
                }
            }
            if (playingOrder.size() == 1) {
                sum = sum - playingOrder.at(0)->getPlayerOrdersList()->size();
                onePlayer  = true;
                break;
            }
        }
    }
}

// Iloggable
string GameEngine::stringToLog() {
    return "";
}

string GameEngine::modeToString(Modes mode) {
    switch(mode) {
        case Modes::STARTUP:
            return "Startup";
        case Modes::PLAY:
            return "Play";
    }
    return "";
}

string GameEngine::phaseToString(Phases phase) {
    switch(phase){
        case Phases::START:
            return "Start";
        case Phases::MAPLOADED:
            return "Map Loaded";
        case Phases::MAPVALIDATED:
            return "Map Validated";
        case Phases::ASSIGNREINFORCEMENT:
            return "Assign Reinforcement";
        case Phases::PLAYERSADDED:
            return "Players Added";
        case Phases::ISSUEORDERS:
            return "Issue Orders";
        case Phases::EXECUTEORDERS:
            return "Execute Orders";
        case Phases::WIN:
            return "Win";
    }
    return "";
}

void GameEngine::transition(Phases phaseToTransition) {
    *phase = phaseToTransition;
    string phaseString = phaseToString(phaseToTransition);
    contentToLog = "Game Engine - changing to phase: " + phaseString;
    notify();
}

vector<Player *> GameEngine::getPlayingOrder() {
    return playingOrder;
}

void GameEngine::gameReset() {
    cout << "Resetting the game..." << endl;
    cout << "\tClearing playing order" << endl;
    // clear playing order list
    playingOrder.clear();

    cout << "\tReturning cards to the deck, ceding territories and removing players..." << endl;
    // return all cards in a player's hand to the deck, cede owned territories, then remove the player
    for (auto &player : players_) {
        if(player != nullptr) {
            if(player->getPlayerCards() != nullptr) {
                player->getPlayerCards()->removeAllCards();
            }
            OrdersList* ol = player -> getPlayerOrdersList();
            int size = ol->size();
            for (int k =0 ; k<size; k++){
                ol->popTopOrder();
            }
            player->removeAllTerritories();
            delete player;
            player = nullptr;
        }
    }
    players_.clear();

    cout << "\tDeleting the map..." << endl;
    // delete map
    if(map_ != nullptr) {
        delete map_;
        map_ = nullptr;
    }

    cout << "\tResetting game's mode and phase..." << endl;
    // reset mode and phase
    *mode = Modes::STARTUP;
    transition(Phases::START);

    cout << "Game was reset successfully" << endl;
}

void GameEngine::gamePlay() {
    Command *command = nullptr;
    string instruction;
    while(instruction != "eof") {
        if(*mode == Modes::STARTUP && *phase == Phases::START) {
            startupPhase();
        }
        if(*mode == Modes::PLAY && *phase == Phases::ASSIGNREINFORCEMENT) {
            mainGameLoop();
        }

        // ask player for replay/quit after end of game
        do {
            cout << "Invalid choice!\nEnter \"replay\" to play again, or \"quit\" to exit the game.\nWhat's your choice?";
            command = commandProcessor->getCommand();
            commandProcessor->validate(command, phase);
            instruction = command->getInstruction();
            if(instruction == "eof") {
                cout << "End of the list of commands!" << endl;
            }
        } while(instruction != "quit" && instruction != "replay" && instruction != "eof");

        // perform the command's required action then save the effect and go to the next phase
        if(instruction == "quit"  && *phase == Phases::WIN) {
            cout << "Goodbye!" << endl;
            return;
        } else if(instruction == "replay"  && *phase == Phases::WIN) {
            gameReset();
        }
    }
}

void GameEngine::resetPlayerStrategy(){
    cout << "\tResetting players' strategy..." << endl;
    for (auto &player : players_) {
        string name = player ->getName();
        if(name.find("Aggressive") != string::npos ) {
            player->setStrategy(strategy::Aggressive);
        } else if(name.find("Human") != string::npos) {
            player->setStrategy(strategy::Human);
        } else if(name.find("Neutral") != string::npos) {
            player->setStrategy(strategy::Neutral);
        } else if(name.find("Cheater") != string::npos) {
            player->setStrategy(strategy::Cheater);
        } else if(name.find("Benevolent") != string::npos) {
            player->setStrategy(strategy::Benevolent);
        }
    }
}


