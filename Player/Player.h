//
// Created by sarah on 2021-09-19.
//

#ifndef COMP345_N11_PLAYER_H
#define COMP345_N11_PLAYER_H

#include <string>
#include <vector>
#include <iostream>

#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Orders/Orders.h"

class Territory;
class Hand;
class Order;
class OrdersList;
using namespace std;

class Player {
private:
    string name;
    vector<Territory*> territories;
    Hand *playerCards;
    OrdersList *playerOrdersList;
    vector<Player*> diplomaticRelations_;
    bool neutral = false;
public:
    Player();
    Player(const string &name);
    Player(string Name, vector<Territory*> & territories);
    Player(const Player &anotherPlayer);

    ~Player();

    void setName (string name);
    string getName() const;

    void setPlayerCards(Hand *&PlayerCards);
    Hand* getPlayerCards() const;

    void setPlayerOrdersList(OrdersList *&ordersList);
    OrdersList *getPlayerOrdersList() const;

    void setTerritories(vector<Territory*> territories);
    vector<Territory*> getTerritories() const;

    vector<Territory*> toDefend();
    vector<Territory*> toAttack();
    void issueOrder();

    Player &operator = (const Player &anotherPlayer);
    friend ostream &operator << (ostream &out, const Player &player);

    void addTerritory(Territory *newTerritory);
    void removeTerritory(Territory *A_Territory);

    void addDiplomaticRelation(Player* player);

    vector<Player*> getRelations() const;
    bool isNeutral();
};


#endif //COMP345_N11_PLAYER_H
