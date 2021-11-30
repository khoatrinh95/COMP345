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
class PlayerStrategy;

enum class Phases{START, MAPLOADED, MAPVALIDATED, PLAYERSADDED, ASSIGNREINFORCEMENT, ISSUEORDERS, EXECUTEORDERS, WIN};
enum class Modes{STARTUP, PLAY};

class GameEngine: public Subject
{
public:
    GameEngine();
    GameEngine(const GameEngine &anotherGameEngine);
    virtual ~GameEngine();
    GameEngine& operator = (GameEngine const &anotherGameEngine);
    friend ostream& operator << (ostream &stream, const GameEngine &gameEngine);
    static void assignToNeutralPlayer(Territory* territory);
    const vector<Player *> &getPlayers();
    void addPlayersToList(Player* player);
    //////////////////////////////////////////////////Sarah GAME PLAY _____ PART 3
    void mainGameLoop();
    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();
    void removePlayer(Player *player);
    vector<Player* >getPlayingOrder();

    ////////////////////////////////////////////////////////////////////////////
    static Deck *deck;
    string getPlayersNames() const;
    string getPlayingOrderPlayersNames() const;
    void startupPhase();
    static vector<PlayerStrategy*>strategyType ;
    void gamePlay();

    // Iloggable
    virtual string stringToLog();

private:
    const int MIN_NUM_PLAYERS;
    const int MAX_NUM_PLAYERS;
    const string MAP_DIRECTORY;
    static Player *neutralPlayer;
    vector<Player *> playingOrder;
    Phases *phase;
    Modes *mode;
    vector<Player*> players_;
    Map* map_ ;
    CommandProcessor *commandProcessor;
    void printTitle();
    void loadMap(string filename);
    void validateMap(Phases *phase);
    void assignTerritories();
    void assignPlayingOrder();
    void initialReinforcement();
    void initialCardDrawing();
    void printPlayPhaseGreeting();
    string modeToString(Modes mode);
    string phaseToString(Phases phase);
    void transition(Phases phaseToTransition);
    string startupMapLoading(string map);
    string startupMapValidation();
    string startupGameInitialization();
    string tournamentPlay(int numberOfMaxTurns);
    void gameReset();
};
#endif //COMP345_N11_GAMEENGINE_H
