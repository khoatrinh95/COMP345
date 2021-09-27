//
// Created by sarah on 2021-09-19.
//

#include "Player.h"
#include "Map.h"
#include "Cards.h"
#include "Orders.h"

// Player default constructor
Player::Player() {
    name = "anonymous";
    territories;
    PlayerCards = new Hand();
    playerOrdersList = new OrdersList();
}

//  one parameter constructor, that takes a name and assign it to the player's name
Player::Player(string Name) {
    this->name = Name;
    this->territories;
    this->PlayerCards = new Hand();
    this->playerOrdersList = new OrdersList();
}
// Copy constructor that do a deep copy of a player's members
Player::Player(const Player &anotherPlayer) {
    this->name = anotherPlayer.name +"_copy";
    this->PlayerCards = new Hand(*anotherPlayer.PlayerCards);
    this->playerOrdersList = new OrdersList(*anotherPlayer.playerOrdersList);
    vector<Territory*>temp_Territory = anotherPlayer.territories;
    for (vector<Territory*>::iterator iter = temp_Territory.begin(); iter!=temp_Territory.end(); ++iter){
        Territory* newTerritory = new Territory(**iter);
        addTerritory(newTerritory);
    }
}

void Player::addTerritory(Territory * newTerritory) {
    this->territories.push_back(newTerritory);
}

void Player::setName(string name) {
    this->name = name;
}

string Player::getName()const {
    return this->name;
}

void Player::setPlayerCards(Hand *&PlayerCards) {
    this->PlayerCards = PlayerCards;
}

Hand *Player::getPlayerCards()const {
    return this->PlayerCards;
}

void Player::setPlayerOrdersList(OrdersList *&ordersList) {
    this->playerOrdersList = ordersList;
}

OrdersList *Player::getPlayerOrdersList()const {
    return playerOrdersList;
}

void Player::setTerritories(vector<Territory *> territories) {
    this->territories = territories;
}

vector<Territory *> Player::getTerritories()const {
    return territories;
}

vector<Territory *> Player::toDefend() {
    vector<Territory *> territories_to_be_defended;
    territories_to_be_defended.push_back(new Territory());
    return territories_to_be_defended;
}

vector<Territory *> Player::toAttack() {
    vector<Territory *> territories_to_be_attacked;
    territories_to_be_attacked.push_back(new Territory());
    return territories_to_be_attacked;
}









