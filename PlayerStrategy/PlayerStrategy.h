//
// Created by Khoa Trinh on 2021-11-21.
//

#ifndef COMP345_N11_PLAYERSTRATEGY_H
#define COMP345_N11_PLAYERSTRATEGY_H

class Player;

#include <vector>
#include <ostream>
using namespace std;
class Territory;
class Player;
class PlayerStrategy {
public:
    virtual vector<Territory*>  toDefend(Player *player) = 0;
    virtual vector<Territory*>  toAttack(Player *player) = 0;
    virtual void issueOrder(Player *player) = 0;
    virtual void print(Player *player)=0;


};
//

class HumanPlayerStrategy : public PlayerStrategy {
public:
    virtual vector<Territory*> toDefend(Player *player);
    virtual vector<Territory*> toAttack(Player *player);
    virtual void issueOrder(Player *player);
    virtual void print(Player *player);

};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    virtual vector<Territory*> toDefend(Player *player);
    virtual vector<Territory*> toAttack(Player *player);
    virtual void issueOrder(Player *player);
    virtual void print(Player *player);

};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
//    BenevolentPlayerStrategy();
    virtual vector<Territory*> toDefend(Player *player);
    virtual vector<Territory*> toAttack(Player *player);
    virtual void issueOrder(Player *player);
    virtual void print(Player *player);

};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
    virtual vector<Territory*>  toDefend(Player *player);
    virtual vector<Territory*>  toAttack(Player *player);
    virtual void issueOrder(Player *player);
    virtual void print(Player *player);

};

class CheaterPlayerStrategy : public PlayerStrategy {
public:
    virtual vector<Territory*> toDefend(Player *player);
    virtual vector<Territory*> toAttack(Player *player);
    virtual void issueOrder(Player *player);
    virtual void print(Player *player);

};

#endif //COMP345_N11_PLAYERSTRATEGY_H
