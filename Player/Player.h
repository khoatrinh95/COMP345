//
// Created by sarah on 2021-10-01.
//

#ifndef COMP345_N11_PLAYER_H
#define COMP345_N11_PLAYER_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include "../Orders/Orders.h"
#include "../PlayerStrategy/PlayerStrategy.h"

class Territory;
class Hand;
class Order;
class OrdersList;
class Card;

enum strategy:int{
    Aggressive,
    Human,
    Neutral,
    Cheater,
    Benevolent
};
class Player {
private:
    string name;
    vector<Territory*> territories;
    Hand *playerCards;
    OrdersList *playerOrdersList;
    vector<Player*> diplomaticRelations_;
    bool neutral;
    int reinforcement_pool;
    PlayerStrategy* ps;


public:
    /**
     * default constructor for Player object
     */
    Player();


    /**
     * parameterized constructor for Player
     * @param name player name
     */
    Player(string name);

    /**
     * parameterized constructor for Player
     * @param Name player name
     * @param territories player list of territories
     */
    Player(string Name, vector<Territory*> & territories);

    /**
     * copy constructor for player
     * @param Name player name
     * @param territories player list of territories
     * @param ps a player strategy
     */

    Player(string Name, vector<Territory*> & territories, PlayerStrategy* ps);

    /**
     * copy constructor for player
     * @param anotherPlayer
     */

    Player(const Player &anotherPlayer);

    /**
     * parameterized constructor for Player
     * @param neutral
     */
    Player(string _name, bool neutral);
    /**
     * destructor for player object
     */
    ~Player();

    /**
     * to add or change player name
     * @param name
     */
    void setName (string name);

    /**
     * to return player name
     * @return player name
     */
    string getName() const;

    /**
     * add or change player hand of cards
     * @param PlayerCards
     */
    void setPlayerCards(Hand *&PlayerCards);

    /**
     * get player hand of cards
     * @return player hand of cards
     */
    Hand* getPlayerCards() const;

    /**
     * add or change player orders list
     * @param ordersList
     */
    void setPlayerOrdersList(OrdersList *&ordersList);

    /**
     * get player orders list
     * @return player orders list
     */
    OrdersList *getPlayerOrdersList() const;


    /**
     * add or change player list of territories
     * @param territories
     */
    void setTerritories(vector<Territory*> territories);

    /**
     * get player list of territories
     * @return player list of territories
     */
    vector<Territory*> getTerritories() const;

    /**
     * set a player number of armies
     * @param armies
     */
    void setReinforcementPool(int armies);

    /**
     * get player number of armies
     * @return
     */
    int getReinforcementPool() const;

    /**
     * adding armies to player reinforcement pool
     */
    void assignReinforcementToPlayer (int );

    /**
     * determine player territory(s) that need to be defended
     * @return list of territories
     */
    vector<Territory*> toDefend();

    /**
     * determine player territory(s) that need to be attacked
     * @return list of territories
     */
    vector<Territory*> toAttack();

    /**
     * add an order to player orders list
     */
    void issueOrder();


    /**
     * defining the assignment operator for player
     * @param anotherPlayer
     * @return a player
     */
    Player &operator = (const Player &anotherPlayer);

    /**
     * overriding the out stream operator for player object
     * @param out out stream
     * @param player
     * @return the outstream for player object
     */
    friend ostream &operator << (ostream &out, const Player &player);

    /**
     * add a territory to player list of territories
     * @param newTerritory
     */
    void addTerritory(Territory *newTerritory);

    /**
 * remove a territory from player list of territories
 * @param A_Territory
 */
    void removeTerritory(Territory *A_Territory);
    /**
     * transfer a territory to player list of territories
     * @param newTerritory
     */
    void transferTerritory (Territory *newTerritory, Player *toTransfer);

    /**
     * Add an enemy player to the list of diplomatic relations for this player
     * @param player
     */
    void addDiplomaticRelation(Player* player);

    /**
     * find the relation between
     * @return
     */
    vector<Player*> getRelations() const;

    /**
     * check if a player represent the system
     * @return true or false
     */
    bool isNeutral();

    void setStrategy(int strategy );
    PlayerStrategy * getStrategy()const;
    static strategy parsePlayerStrategy(string strStrategy);
    Player(string name, strategy playingStrategy);
    void removeAllTerritories();

    //Thong
    std::vector<Territory*> getOwnTerritoriesWithMovableArmies() const;
    Hand* hand_;
    bool committed_;
    void addOrder(Order* order);



};



#endif //COMP345_N11_PLAYER_H
