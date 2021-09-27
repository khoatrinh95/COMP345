//
// Created by sarah on 2021-09-19.
//

#ifndef COMP345_N11_PLAYER_H
#define COMP345_N11_PLAYER_H
#include <string>
#include <vector>
#include <list>

using namespace std;

#include "Map.h"
#include "Cards.h"
#include "Orders.h"

class Player {
private:
    string name;
    vector<Territory *>territories;
    Hand* PlayerCards;
    OrdersList* playerOrdersList;
public:
    Player();
    Player(string Name);
    Player(const Player& anotherPlayer );

    void setName (string name );
    string getName()const;

    void setPlayerCards(Hand*& PlayerCards);
    Hand* getPlayerCards()const;

    void setPlayerOrdersList(OrdersList*& ordersList);
    OrdersList* getPlayerOrdersList()const;

    void setTerritories(vector<Territory*> territories);
    vector<Territory*>getTerritories()const;

    vector<Territory *> toDefend();
    vector<Territory *> toAttack();
    void issueOrder();

    void addTerritory(Territory*);










};


#endif //COMP345_N11_PLAYER_H
