//
// Created by sarah on 2021-09-19.
//

#ifndef COMP345_N11_PLAYER_H
#define COMP345_N11_PLAYER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>

using namespace std;

#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Orders.h"

class Player {
private:
    string name;
    vector<Territory*> territories;
    Hand *playerCards;
    OrdersList *playerOrdersList;
public:
    Player();
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


    void addTerritory(Territory*);










};


#endif //COMP345_N11_PLAYER_H
