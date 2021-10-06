//
// Created by sarah on 2021-09-19.
//

#ifndef COMP345_N11_PLAYER_H
#define COMP345_N11_PLAYER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>

#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Orders/Orders.h"
using namespace std;
class Player {
private:
    std::string name;
    std::vector<Territory*> territories;
    Hand *playerCards;
    OrdersList *playerOrdersList;
    std::vector<Player*> diplomaticRelations_;
    std::vector<Territory*> ownedTerritories_;
public:
    Player();
    Player(string Name, vector<Territory*> & territories);
    Player(const Player &anotherPlayer);

    Player(const string &name);

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
    void addOwnedTerritory(Territory* territory);
    void addTerritory(Territory *newTerritory);
    void removeTerritory(Territory *A_Territory);
    void removeOwnedTerritory(Territory* territory);
    void addDiplomaticRelation(Player* player);
    vector<Player*> getRelations() const;
};


#endif //COMP345_N11_PLAYER_H
