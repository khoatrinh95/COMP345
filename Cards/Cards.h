//
// Created by Ricky Lam on 2021-09-26.
//

#ifndef COMP345_CARDS_H
#define COMP345_CARDS_H

#include <string>
#include "../Orders/Orders.h"
#include "../Map/Map.h"
#include <vector>
using namespace std;

class Player;
class Order;
class Card;
class Territory;

class Card{
public:
    //constructors and destructors
    Card();
    Card(string type);
    ~Card();

    //copy constructor and operator overloads
    Card(const Card& c);
    Card& operator =(const Card& c);
    friend std::ostream& operator<<(std::ostream& stream, const Card& c);

    Card* play(Player *&player); //play function as per assignment handout
    void testType(); //guard function for constructor
    string printCard() const; //for debugging, unused

    string getType() const; //accessor
    void setType(string typer); //mutator

    void useCardtoCreateOrder(Player *player, int numArmies, Territory *source,Territory *target);

private:
    string type;
};

class Deck{
public:
    //constructors and destructors
    Deck();
    Deck(vector<Card*> &cards);
    ~Deck();

    //copy constructor and operator overloads
    Deck(const Deck& d);
    Deck& operator =(const Deck& d);
    friend std::ostream& operator<<(std::ostream& stream, const Deck& d);

    void addCard(Card* carder); //adds a card to the deck manually
    void addCard(string type); //creates a card of the type passed in parameter then adds to deck
    Card* draw(); //draws a card from the deck as requested from the assignment handout

    int getSize() const; //function to see how big the deck is at the moment
    string printDeck() const; //function to see all cards in deck, also used in << operator

private:
    vector<Card*> cards;
};

class Hand{
public:
    //constructors and destructors
    Hand();
    Hand(const vector<Card*> &cards);
    ~Hand();

    //copy constructor and operator overloads
    Hand(const Hand& h);
    Hand& operator =(const Hand& h);
    friend std::ostream& operator<<(std::ostream& stream, const Hand& h);

    void drawFromDeck(Deck* deck); //removes a card randomly from the deck and adds it to the player's hand
    void playAllCards(Deck* deck, Player *&player); //plays all cards in the player's hand
    bool playOneCard(int position, Deck* deck, Player *&player); //plays a specific card, use printHand or << operator to find out which cards are at what position

    int getSize() const; //function to see the size of the player's hand
    string printHand() const; //prints out all cards on hand
    vector<Card *> getHand() const;
    void removeCard(Card * card);

private:
    vector<Card*> hand;

};

class DiplomacyCard : public Card
{
public:
    Order* play() const;
    Card* clone() const;

protected:
    std::ostream &print_(std::ostream &output) const;
    Order* buildOrder_() const;
};

#endif //COMP345_CARDS_H