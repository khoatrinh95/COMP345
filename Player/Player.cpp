    //
    // Created by sarah on 2021-09-19.
    //

#include "Player.h"
#include <iostream>
#include "../GameEngine/GameEngine.h"

using namespace std;

/**
 * default constructor for player
 */
Player::Player() {
    name = "anonymous";
    playerCards = new Hand();
    playerOrdersList = new OrdersList();
    vector<Territory*> territories;
}


/**
 * parameter constructor, that takes a name and assign it to the player's name, territories and assign it to player
   territories
  * @param Name
  * @param territories
  */
Player::Player(string Name, vector<Territory *> &territories) {
    this->name = Name;
    this->territories = territories;
    this->playerCards = new Hand();
    this->playerOrdersList = new OrdersList();
    }


/**
 * a constructor that takes only player name as parameter
 * @param name
  */
Player::Player(const string &name) : name(name) {}

/**
 * Copy constructor that do a deep copy of a player's members
 * @param anotherPlayer
 */
Player::Player(const Player &anotherPlayer) {
    this->name = anotherPlayer.name +"_copy";
    this->playerCards = new Hand(*anotherPlayer.playerCards);
    this->playerOrdersList = new OrdersList(*anotherPlayer.playerOrdersList);
    vector<Territory*>temp_Territory = anotherPlayer.territories;
    for (vector<Territory*>::iterator iter = temp_Territory.begin(); iter!=temp_Territory.end(); ++iter){
        Territory* newTerritory = new Territory(**iter);
        newTerritory->setOwner(this);
        addTerritory(newTerritory);
    }
}

/**
 * destructor of player object
 */
Player::~Player(){
    cout<<"~player() destructing a player whose name is "<< name<<endl;
    for (auto &territory :territories){
        territory->removeOwner();
    }

    if (playerCards != nullptr) {
        delete playerCards;
    }

    if (playerOrdersList != nullptr) {
        delete playerOrdersList;
    }
}

/**
 * to add or change player name
 * @param name
 */
void Player::setName(string name) {
    this->name = name;
}

/**
 * to return player name
 * @return player name
 */
string Player::getName() const {
    return this->name;
}

/**
 * add or change player hand of cards
 * @param PlayerCards
 */
void Player::setPlayerCards(Hand *&PlayerCards) {
    this->playerCards = PlayerCards;
}

/**
 * get player hand of cards
 * @return player hand of cards
 */
Hand *Player::getPlayerCards() const {
    return this->playerCards;
}

/**
 * add or change player orders list
 * @param ordersList
 */

void Player::setPlayerOrdersList(OrdersList *&ordersList) {
    this->playerOrdersList = ordersList;
}

/**
 * get player orders list
 * @return player orders list
 */
OrdersList *Player::getPlayerOrdersList()const {
    return playerOrdersList;
}

/**
 * add or change player list of territories
 * @param territories
 */
void Player::setTerritories(vector<Territory*> territories) {
    this->territories = territories;
}

/**
 * get player list of territories
 * @return player list of territories
 */
vector<Territory*> Player::getTerritories() const {
    return territories;
}

/**
 * determine player territory(s) that need to be defended
 * @return list of territories
 */
vector<Territory*> Player::toDefend() {
    Continent *continent = new Continent(12,"South America",7);
    vector<Territory*> territories_to_be_defended;
    territories_to_be_defended.push_back(new Territory(12,"Colombia", 4,continent));
    territories_to_be_defended.push_back(new Territory(21,"Brazil", 2,continent));
    territories_to_be_defended.push_back(new Territory(100,"Argentina", 1,continent));
    return territories_to_be_defended;
}

/**
 * determine player territory(s) that need to be attacked
 * @return list of territories
 */
vector<Territory*> Player::toAttack() {
    Continent *continent = new Continent(10,"Europe",5);
    vector<Territory*> territories_to_be_attacked;
    territories_to_be_attacked.push_back(new Territory(3,"England", 5,continent));
    territories_to_be_attacked.push_back(new Territory(7,"Italy", 3,continent));
    territories_to_be_attacked.push_back(new Territory(10,"Greece", 2,continent));
    return territories_to_be_attacked;
}

/**
 * add an order to player orders list
 */
void Player::issueOrder() {
    DeployOrder *anOrder = new DeployOrder();
    playerOrdersList->add(anOrder);
}

/**
 * defining the assignment operator for player
 * @param anotherPlayer
 * @return a player
 */
Player &Player::operator=(const Player &anotherPlayer) {
    if (this != & anotherPlayer){
        this->~Player();
        territories.clear();
        this->name = anotherPlayer.name +"_copy";
        this->playerCards = new Hand(*anotherPlayer.playerCards);
        this->playerOrdersList = new OrdersList(*anotherPlayer.playerOrdersList);
        vector<Territory*>temp_Territory = anotherPlayer.territories;
        for (vector<Territory*>::iterator iter = temp_Territory.begin(); iter!=temp_Territory.end(); ++iter){
            Territory* newTerritory = new Territory(**iter);
            newTerritory->setOwner(this);
            addTerritory(newTerritory);
        }
    }
    return *this;
}

/**
 * overriding the out stream operator for player object
 * @param out out stream
 * @param player
 * @return the outstream for player object
 */
ostream &operator<<(ostream &out, const Player &player) {
    if (player.territories.empty()){
        out << "Player name is "<<player.name<<", and he has no territories"<<endl;
    }else{
        out << "Player name is "<< player.name<< ", and he has the following territories:"<<endl<<"\t";
        for (auto &territory : player.territories){
            out << *territory << endl<<"\t";
        }
        out<<endl;
    }

    if (player.playerCards->getSize()==0){
        out<<"This player does not have a hand of cards"<<endl;
    } else{
        out<<"player hand of cards is :"<<endl;
        out<<*player.playerCards;
    }

    if (player.playerOrdersList->size()== 0){
        out<<"This player does not have any orders in his list"<<endl;
    } else{
        out <<"Player orders list contain these orders"<<endl;
        out <<*player.playerOrdersList<<endl;
    }
    out<< endl;

    return out;
}

/**
 * add a territory to player list of territories
 * @param newTerritory
 */
void Player::addTerritory(Territory *newTerritory) {
    this->territories.push_back(newTerritory);
}

/**
 * remove a territory from player list of territories
 * @param A_Territory
 */
void Player::removeTerritory(Territory *A_Territory) {
    for (int i = 0; i<territories.size();i++){
        if (territories.at(i)->getId()==A_Territory->getId()){
            territories.erase(next(begin(territories), + i));
        }
    }
}

/**
 * Add an enemy player to the list of diplomatic relations for this player
 * @param player
 */
void Player::addDiplomaticRelation(Player* player){
    diplomaticRelations_.push_back(player);
}

/**
 * find player relationships with other players
 * @return
 */
vector<Player *> Player::getRelations() const {
    return diplomaticRelations_;
}

/**
 * check if a player represent the system
 * @return true or false
 */
bool Player::isNeutral() {
    return neutral;
}




