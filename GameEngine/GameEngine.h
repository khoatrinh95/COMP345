//
// Created by Khoa Trinh on 2021-09-29.
//

#ifndef COMP345_N11_GAMEENGINE_H
#define COMP345_N11_GAMEENGINE_H

#include "../cards/Cards.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../CommandProcessing/CommandProcessing.h"
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
class Player;
class Map;
class Territory;
class Deck;
class CommandProcessor;


enum class Phases{START, MAPLOADED, MAPVALIDATED, PLAYERSADDED, ASSIGNREINFORCEMENT, ISSUEORDERS, EXECUTEORDERS, WIN};
enum class Modes{STARTUP, PLAY};


//class StartUp {
//private:
//    static void printTitle();
//    static void addPlayers();
//public:
//    // Did not implement copy constructor, assignment overload and ostream insertion as class does not have any member
//    // It does not make sense to have those methods
//    static void loadMap(Phases *phase);
//    static void validateMap(Phases *phase);
//    static void createPlayers(Phases *phase);
//    static void assignTerritories(Phases *phase);
//    static void startUp(Phases *phase);
//};

class Play {
private:

    static void printPlayPhaseGreeting();
public:
    // Did not implement copy constructor, assignment overload and ostream insertion as class does not have any member
    // It does not make sense to have those methods
    static void assignArmies(Phases *phase);
    static void issueOrders(Phases *phase);
    static void executeOrders(Phases *phase);
    static void playGame(Phases *phase);
};

namespace HelperFunctions {
    void printOptions(vector<string>);
    bool isNumber(const std::string&);
    void takeInput(Phases *phase, Phases);
}

class GameEngine
{
public:
    GameEngine();
    GameEngine(const GameEngine &anotherGameEngine);
    virtual ~GameEngine();
    GameEngine& operator = (GameEngine const &anotherGameEngine);
    friend ostream& operator << (ostream &stream, const GameEngine &gameEngine);
    Player* getOwnerOf(Territory* territory) const;
    static void assignToNeutralPlayer(Territory* territory);
    void playGame();
    const vector<Player *> &getPlayers();
    void addPlayersToList(Player* player);
    void clearPlayerList();
    void assignRandomCardsToPlayers();
    //////////////////////////////////////////////////Sarah GAME PLAY _____ PART 3
    void mainGameLoop();
    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();
    void removePlayer(Player *player);

    ////////////////////////////////////////////////////////////////////////////
    static Deck *deck;
    string getPlayersNames() const;
    string getPlayingOrderPlayersNames() const;
    void startupPhase();

private:
    const string mapDirectory;
    static Player *neutralPlayer;
    vector<Player *> playingOrder;
    Phases *phase;
    vector<Player*> players_;
    Map* map_ ;
    CommandProcessor *commandProcessor;
    void printTitle();
    void addPlayers();
    void loadMap(string filename);
    void validateMap(Phases *phase);
    void createPlayers(Phases *phase);
    void assignTerritories();
    void assignPlayingOrder();
    void initialReinforcement();
    void initialCardDrawing();
};
#endif //COMP345_N11_GAMEENGINE_H
