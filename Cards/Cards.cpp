//
// Created by Ricky Lam on 2021-09-26.
//

#include<iostream>
#include <sstream>
#include "Cards.h"

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

Card* Card::play() {
    cout << "Played a card of type " << type << endl;
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

Deck::Deck(vector<Card*> cards) : cards(cards){
}

Deck::Deck(const Deck& d){
    cout << "Inside copy constructor of Deck" << endl;
    cards = d.cards;
}

Deck::~Deck() {
    for (int i = 0; i < cards.size(); i++) {
        delete[] cards[i];
    }
}

Deck& Deck::operator=(const Deck& d){
    cout << "Inside operator = of Deck" << endl;
    cards = d.cards;
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const Deck& d){
    return stream << "Deck details, size: " << d.getSize() << endl << "Cards in deck: " << d.printDeck();
}

int Deck::getSize() const { return cards.size(); }

string Deck::printDeck() const{
    std::ostringstream buffer;
    if (cards.size() <= 0) {
        return "Deck is empty!";
    }
    for (int i = 0; i < cards.size()+1; i++) {
        buffer << "[Card " << (i + 1) << ": " << cards[i]->getType() << "]\t";
        if (i != 0 && i % 3 == 0) {
            buffer << "\n";
        }
    }
    return buffer.str();
}

void Deck::addCard(Card* card) {
    cards.push_back(card);
}

void Deck::addCard(string type) {
    Card toAdd(type);
    Card* addPointer = &toAdd;
    cards.push_back(addPointer);
    addPointer = nullptr;
}

Card* Deck::draw() {
    int random(rand() % cards.size());
    Card* toReturn = cards[random];
    cards.erase(cards.begin() + random);
    return toReturn;
}

Hand::Hand(){
}

Hand::Hand(const vector<Card*> cards) : hand(cards){
}

Hand::Hand(const Hand& h){
    cout << "Inside copy constructor of Hand" << endl;
    hand = h.hand;
}

Hand::~Hand() {
    for (int i = 0; i < hand.size()+1; i++) {
        delete[] hand[i];
    }
}

Hand& Hand::operator=(const Hand& h){
    cout << "Inside operator = of Hand" << endl;
    hand = h.hand;
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const Hand& h){
    return stream << "Hand details, size: " << h.getSize() << endl << "Cards in hand: " << h.printHand();
}

int Hand::getSize() const { return hand.size(); }

string Hand::printHand() const{
    std::ostringstream buffer;
    if (hand.size() <= 0) {
        return "Hand is empty!";
    }
    for (int i = 0; i < hand.size()+1; i++) {
        buffer << "[Card " << (i + 1) << ": " << hand[i]->getType() << "]\t";
        if (i != 0 && i % 3 == 0) {
            buffer << "\n";
        }
    }
    return buffer.str();
}

void Hand::drawFromDeck(Deck* deck) {
    cout << "Drawing a card from the deck..." << endl;
    hand.push_back(deck->draw());
}

void Hand::playAllCards(Deck* deck) {
    cout << "Playing all cards in hand!" << endl;
    int size = hand.size();
    for (int i = 0; i < size; i++) {
        deck->addCard(hand[0]->play());
        hand.erase(hand.begin());
    }
}