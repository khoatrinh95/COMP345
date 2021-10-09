//
// Created by Khoa Trinh on 2021-09-29.
//

#ifndef COMP345_N11_GAMEENGINE_H
#define COMP345_N11_GAMEENGINE_H

#include "../cards/Cards.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../Map/Map.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;
class Player;
enum class Phases{START, MAPLOADED, MAPVALIDATED, PLAYERSADDED, ASSIGNREINFORCEMENT, ISSUEORDERS, EXECUTEORDERS, WIN};


class StartUp {
private:
    static void printTitle();
    static void addPlayers();
public:
    // Did not implement copy constructor, assignment overload and ostream insertion as class does not have any member
    // It does not make sense to have those methods
    static void loadMap(Phases *phase);
    static void validateMap(Phases *phase);
    static void createPlayers(Phases *phase);
    static void assignTerritories(Phases *phase);
    static void startUp(Phases *phase);
};

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
    static Player* getOwnerOf(Territory* territory);
    static void assignToNeutralPlayer(Territory* territory);
    void playGame();
    static const vector<Player *> &getPlayers();
    static void addPlayersToList(Player* player);
    static void clearPlayerList();
    static void assignRandomCardsToPlayers();
private:
    Phases *phase;
    static std::vector<Player*> players_;

};
#endif //COMP345_N11_GAMEENGINE_H
