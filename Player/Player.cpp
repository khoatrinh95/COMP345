//
// Created by sarah on 2021-09-19.
//

#include "Player.h"
#include <iostream>
#include "../GameEngine/GameEngine.h"

using namespace std;
// Player default constructor
Player::Player() {
    name = "anonymous";
    playerCards = new Hand();
    playerOrdersList = new OrdersList();
    vector<Territory*> territories;
}

// parameter constructor, that takes a name and assign it to the player's name, territories and assign it to player
// territories
Player::Player(string Name, vector<Territory *> &territories) {
    this->name = Name;
    this->territories = territories;
    this->playerCards = new Hand();
    this->playerOrdersList = new OrdersList();

}
// a constructor that takes only player name as parameter
Player::Player(const string &name) : name(name) {}

// Copy constructor that do a deep copy of a player's members
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

Player::~Player(){
    cout<<"~player() destructing a player whose name is "<< name<<endl;
    for (auto &territory :territories){
        territory->removeOwner();
    }
    delete playerCards;
    delete playerOrdersList;
}

void Player::setName(string name) {
    this->name = name;
}

string Player::getName() const {
    return this->name;
}

void Player::setPlayerCards(Hand *&PlayerCards) {
    this->playerCards = PlayerCards;
}

Hand *Player::getPlayerCards() const {
    return this->playerCards;
}

void Player::setPlayerOrdersList(OrdersList *&ordersList) {
    this->playerOrdersList = ordersList;
}

OrdersList *Player::getPlayerOrdersList()const {
    return playerOrdersList;
}

void Player::setTerritories(vector<Territory*> territories) {
    this->territories = territories;
}

vector<Territory*> Player::getTerritories() const {
    return territories;
}

vector<Territory*> Player::toDefend() {
    Continent *continent = new Continent(12,"South America",7);

    vector<Territory*> territories_to_be_defended;
    territories_to_be_defended.push_back(new Territory(12,"Colombia", 4,continent));
    territories_to_be_defended.push_back(new Territory(21,"Brazil", 2,continent));
    territories_to_be_defended.push_back(new Territory(100,"Argentina", 1,continent));
    return territories_to_be_defended;
}

vector<Territory*> Player::toAttack() {

    Continent *continent = new Continent(10,"Europe",5);
    vector<Territory*> territories_to_be_attacked;
    territories_to_be_attacked.push_back(new Territory(3,"England", 5,continent));
    territories_to_be_attacked.push_back(new Territory(7,"Italy", 3,continent));
    territories_to_be_attacked.push_back(new Territory(10,"Greece", 2,continent));

    return territories_to_be_attacked;
}

// Thong's part
void Player::issueOrder() {
    DeployOrder *anOrder = new DeployOrder();
    playerOrdersList->add(anOrder);
}

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

void Player::addTerritory(Territory *newTerritory) {
    this->territories.push_back(newTerritory);
}

void Player::removeTerritory(Territory *A_Territory) {
    for (int i = 0; i<territories.size();i++){
        if (territories.at(i)->getId()==A_Territory->getId()){
            territories.erase(next(begin(territories), + i));
        }
    }
}

// Add an enemy player to the list of diplomatic relations for this player
void Player::addDiplomaticRelation(Player* player)
{
    diplomaticRelations_.push_back(player);
}

std::vector<Player *> Player::getRelations() const
{
    return diplomaticRelations_;
}

bool Player::isNeutral() {
    return neutral;
}




