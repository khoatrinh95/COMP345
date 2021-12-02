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


    virtual PlayerStrategy* clone() const = 0;
    friend std::ostream &operator<<(std::ostream &output, const PlayerStrategy &strategy);
protected:
    virtual std::ostream &print_(std::ostream &output) const = 0;
};
//


class HumanPlayerStrategy : public PlayerStrategy {
    PlayerStrategy *clone() const;

    std::vector<Territory *> toDefend(const Player *player) const;

    std::vector<Territory *> toAttack(const Player *player) const;

    void issueOrder(Player *player);

protected:
    std::ostream &print_(std::ostream &output) const;

private:
    void deployReinforcements_(Player *player, std::vector<Territory *> territoriesToDefend);

    void issueAdvance_(Player *player, std::vector<Territory *> territoriesToDefend);

    void playCard_(Player *player, std::vector<Territory *> territoriesToDefend);
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    virtual vector<Territory*> toDefend(Player *player);
    virtual vector<Territory*> toAttack(Player *player);
    virtual void issueOrder(Player *player);
    virtual void print(Player *player);

protected:
    std::ostream &print_(std::ostream &output) const;

private:
    bool deployToTopTerritory_(Player* player, std::vector<Territory*> territoriesToDefend);
    bool attackFromTopTerritory_(Player* player, Territory* attackFrom, std::vector<Territory*> territoriesToAttack);
    bool advanceToRandomTerritory_(Player* player, std::vector<Territory*> territoriesToDefend);
    bool playCard_(Player* player, std::vector<Territory*> territoriesToDefend);
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
