//
// Created by Ricky Lam on 2021-09-26.
//

#include<iostream>
#include <sstream>
#include "Cards.h"
#include "../Player/Player.cpp"
#include "../Orders/Orders.cpp"
#include <ctime>

using namespace std;

Card::Card(){
}

Card::Card(string type) {
    this->type = type;
    testType();
}

Card::Card(const Card& c){
    cout << "Inside copy constructor of Card" << endl;
    type = c.type;
    testType();
}

Card::~Card() {
    cout << "Inside destructor of Card of type " << type << endl;
}

Card& Card::operator=(const Card& c){
    cout << "Inside operator = of Card" << endl;
    type = c.type;
    testType();
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const Card& c){
    return stream << "Card details: [type: " << c.getType() << "]" << endl;
}

string Card::getType() const {
    return type;
}

void Card::setType(string typer) {
    type = typer;
}

string Card::printCard() const{
    std::ostringstream buffer;
    buffer << type << " type card";
    return buffer.str();
}

Card* Card::play(Player *&player) {
    cout << "Played a card of type " << type << endl;
    OrdersList* lister = player->getPlayerOrdersList();
    if (type.compare("bomb") == 0) {
        lister->add(new BombOrder());
    } else if (type.compare("reinforcement") == 0) {
        lister->add(new AdvanceOrder());
    } else if (type.compare("blockade") == 0) {
        lister->add(new BlockadeOrder());
    } else if (type.compare("airlift") == 0) {
        lister->add(new AirliftOrder());
    } else if (type.compare("diplomacy") == 0) {
        lister->add(new NegotiateOrder());
    }
    return this;
}

void Card::testType() {
    string validTypes[] = {"bomb", "reinforcement", "blockade", "airlift", "diplomacy"};
    bool correct = false;
    for (int i = 0; i < validTypes->size()+1; i++) {
        if (type.compare(validTypes[i]) == 0) {
            correct = true;
            break;
        }
    }
    if (correct == false) {
        cout << "Invalid card type\n";
        exit(1);
    }

}

Deck::Deck(){
}

Deck::Deck(vector<Card*> &cards) : cards(cards){
}

Deck::Deck(const Deck& d){
    cout << "Inside copy constructor of Deck" << endl;
    vector<Card*> copyOfDeck;
    for (int i = 0; i < d.cards.size(); i++) {
        copyOfDeck.push_back(new Card(*d.cards[i]));
    }
    cards = copyOfDeck;
}

Deck::~Deck() {
    cout << "Inside destructor of Deck" << endl;
    for (auto card : cards) {
        delete card;
    }
    cards.clear();
}

Deck& Deck::operator=(const Deck& d){
    cout << "Inside operator = of Deck" << endl;
    vector<Card*> copyOfDeck;
    for (int i = 0; i < d.cards.size(); i++) {
        copyOfDeck.push_back(new Card(*d.cards[i]));
    }
    cards = copyOfDeck;
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const Deck& d){
    return stream << "Deck details, size: " << d.getSize() << endl << "Cards in deck: \n" << d.printDeck();
}

int Deck::getSize() const { return cards.size(); }

string Deck::printDeck() const{
    std::ostringstream buffer;
    bool newLine = false;
    if (cards.size() <= 0) {
        return "Deck is empty!";
    }
    for (int i = 0; i < cards.size(); i++) {
        newLine = false;
        buffer << "[Card " << (i + 1) << ": " << cards[i]->getType() << "]\t\t";
        if ((i+1) % 3 == 0) {
            buffer << "\n";
            newLine = true;
        }
    }
    if (!newLine) { buffer << "\n"; }
    return buffer.str();
}

void Deck::addCard(Card* card) {
    cards.push_back(card);
}

void Deck::addCard(string type) {
    Card toAdd(type);
    Card* addPointer = &toAdd;
    cards.push_back(addPointer);
}

Card* Deck::draw() {
    srand((int)time(0));
    int random(rand() % cards.size());
    Card* toReturn = cards[random];
    cards.erase(cards.begin() + random);
    return toReturn;
}

Hand::Hand(){
}

Hand::Hand(const vector<Card*> &cards) : hand(cards){
}

Hand::Hand(const Hand& h){
    cout << "Inside copy constructor of Hand" << endl;
    vector<Card*> copyOfHand;
    for (int i = 0; i < h.hand.size(); i++) {
        copyOfHand.push_back(new Card(*h.hand[i]));
    }
    hand = copyOfHand;
}

Hand::~Hand() {
    cout << "Inside destructor of Hand" << endl;
    for (auto card : hand) {
        delete card;
    }
    hand.clear();
}

Hand& Hand::operator=(const Hand& h){
    cout << "Inside operator = of Hand" << endl;
    vector<Card*> copyOfHand;
    for (int i = 0; i < h.hand.size(); i++) {
        copyOfHand.push_back(new Card(*h.hand[i]));
    }
    hand = copyOfHand;
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const Hand& h){
    return stream << "Hand details, size: " << h.getSize() << endl << "Cards in hand: \n" << h.printHand();
}

int Hand::getSize() const { return hand.size(); }

string Hand::printHand() const{
    bool newLine = false;
    std::ostringstream buffer;
    if (hand.size() <= 0) {
        return "Hand is empty!";
    }
    for (int i = 0; i < hand.size(); i++) {
        newLine = false;
        buffer << "[Card " << (i + 1) << ": " << hand[i]->getType() << "]\t\t";
        if ((i+1)% 3 == 0) {
            buffer << "\n";
            newLine = true;
        }
    }
    if (!newLine) { buffer << "\n"; }
    return buffer.str();
}

void Hand::drawFromDeck(Deck* deck) {
    cout << "Drawing a card from the deck..." << endl;
    hand.push_back(deck->draw());
}

void Hand::playAllCards(Deck* deck, Player *&player) {
    cout << "Playing all cards in hand!" << endl;
    int size = hand.size();
    for (int i = 0; i < size; i++) {
        deck->addCard(hand[0]->play(player));
        hand.erase(hand.begin());
    }
}

bool Hand::playOneCard(int position, Deck* deck, Player *&player) {
    if (position > hand.size() || position < 1) {
        cout << "Invalid position entered.\n";
        return false;
    } else {
        deck->addCard(hand[position-1]->play(player));
        hand.erase(hand.begin() + position-1);
        return true;
    }

}