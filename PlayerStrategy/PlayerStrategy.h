//
// Created by Khoa Trinh on 2021-11-21.
//

#ifndef COMP345_N11_PLAYERSTRATEGY_H
#define COMP345_N11_PLAYERSTRATEGY_H

class Player;

class PlayerStrategy {
public:
    PlayerStrategy();
    PlayerStrategy(Player* p);
    virtual ~PlayerStrategy();
    void setPlayer(Player* p);
    virtual void toDefend() = 0;
    virtual void toAttack() = 0;
    virtual void issueOrder() = 0;

private:
    Player* player;
};

class HumanPlayer : public PlayerStrategy {
public:
    virtual void toDefend();
    virtual void toAttack();
    virtual void issueOrder();
};

class AgressivePlayer : public PlayerStrategy {
public:
    virtual void toDefend();
    virtual void toAttack();
    virtual void issueOrder();
};

class BenevolentPlayer : public PlayerStrategy {
public:
    virtual void toDefend();
    virtual void toAttack();
    virtual void issueOrder();
};

class NeutralPlayer : public PlayerStrategy {
public:
    virtual void toDefend();
    virtual void toAttack();
    virtual void issueOrder();
};

class CheaterPlayer : public PlayerStrategy {
public:
    virtual void toDefend();
    virtual void toAttack();
    virtual void issueOrder();
};

#endif //COMP345_N11_PLAYERSTRATEGY_H
