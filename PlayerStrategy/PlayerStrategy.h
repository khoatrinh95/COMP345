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
// NOTE: no copy, assignment operators for PlayerStrategy as it doesn't have any attribute
// does not make sense to have those methods

    friend ostream &operator<<(ostream &out, const PlayerStrategy &ps);
};
//

class HumanPlayerStrategy : public PlayerStrategy {
public:
    virtual vector<Territory*> toDefend(Player *player);
    virtual vector<Territory*> toAttack(Player *player);
    virtual void issueOrder(Player *player);
    virtual void print(Player *player);
    friend ostream &operator<<(ostream &out, const HumanPlayerStrategy &ps);
private:
    void deployReinforcements_(Player *player, std::vector<Territory *> territoriesToDefend);

    void issueAdvance_(Player *player, std::vector<Territory *> territoriesToDefend);
    bool playCard_(Player* player);
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    virtual vector<Territory*> toDefend(Player *player);
    virtual vector<Territory*> toAttack(Player *player);
    virtual void issueOrder(Player *player);
    virtual void print(Player *player);
    friend ostream &operator<<(ostream &out, const AggressivePlayerStrategy &ps);
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
//    BenevolentPlayerStrategy();
    virtual vector<Territory*> toDefend(Player *player);
    virtual vector<Territory*> toAttack(Player *player);
    virtual void issueOrder(Player *player);
    virtual void print(Player *player);
    friend ostream &operator<<(ostream &out, const BenevolentPlayerStrategy &ps);
};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
    virtual vector<Territory*>  toDefend(Player *player);
    virtual vector<Territory*>  toAttack(Player *player);
    virtual void issueOrder(Player *player);
    virtual void print(Player *player);
    friend ostream &operator<<(ostream &out, const NeutralPlayerStrategy &ps);
};

class CheaterPlayerStrategy : public PlayerStrategy {
public:
    virtual vector<Territory*> toDefend(Player *player);
    virtual vector<Territory*> toAttack(Player *player);
    virtual void issueOrder(Player *player);
    virtual void print(Player *player);
    friend ostream &operator<<(ostream &out, const CheaterPlayerStrategy &ps);
};

#endif //COMP345_N11_PLAYERSTRATEGY_H
