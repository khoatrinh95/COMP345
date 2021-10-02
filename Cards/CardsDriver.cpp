//
// Created by Ricky Lam on 2021-09-27.
//

#include <iostream>
#include "Cards.cpp"

using namespace std;

void CardsDriver(){
    Card bombCard("bomb");
    Card reinforcementCard("reinforcement");
    Card blockadeCard("blockade");
    Card airliftCard("airlift");
    Card diplomacyCard("diplomacy");
    Card* bombCardptr = &bombCard;
    Card* reinforcementCardptr = &reinforcementCard;
    Card* blockadeCardptr = &blockadeCard;
    Card* airliftCardptr = &airliftCard;
    Card* diplomacyCardptr = &diplomacyCard;

    //cout << bombCard << reinforcementCard << blockadeCard << airliftCard << diplomacyCard << endl;

    Card bombCard2(*bombCardptr);
    Card reinforcementCard2(*reinforcementCardptr);
    Card blockadeCard2(*blockadeCardptr);
    Card airliftCard2(*airliftCardptr);
    Card diplomacyCard2(*diplomacyCardptr);
    Card* bombCard2ptr = &bombCard2;
    Card* reinforcementCard2ptr = &reinforcementCard2;
    Card* blockadeCard2ptr = &blockadeCard2;
    Card* airliftCard2ptr = &airliftCard2;
    Card* diplomacyCard2ptr = &diplomacyCard2;

    //cout << bombCard2 << reinforcementCard2 << blockadeCard2 << airliftCard2 << diplomacyCard2 << endl;

    Deck cardDeck;
    Deck* cardDeckptr = &cardDeck;
    cardDeckptr->addCard(bombCardptr);
    cardDeckptr->addCard(bombCard2ptr);
    cardDeckptr->addCard(reinforcementCardptr);
    cardDeckptr->addCard(reinforcementCard2ptr);
    cardDeckptr->addCard(blockadeCardptr);
    cardDeckptr->addCard(blockadeCard2ptr);
    cardDeckptr->addCard(airliftCardptr);
    cardDeckptr->addCard(airliftCard2ptr);
    cardDeckptr->addCard(diplomacyCardptr);
    cardDeckptr->addCard(diplomacyCard2ptr);

    Deck cardDeckCopy(cardDeck);
    Deck* cardDeckCopyptr = &cardDeckCopy;
    Deck cardDeckCopy2;
    cardDeckCopy2 = *cardDeckCopyptr;
    Deck* cardDeckCopy2ptr = &cardDeckCopy2;
    cout << *cardDeckptr;
    cout << *cardDeckCopyptr;
    cout << *cardDeckCopy2ptr;

    Hand cardHand;
    Hand* cardHandptr = &cardHand;

    for (int i = 0; i < 6; i++){
        cardHandptr->drawFromDeck(cardDeckptr);
    }
    Hand cardHandCopy(*cardHandptr);
    Hand* cardHandCopyptr = &cardHandCopy;
    Hand cardHandCopy2;
    cardHandCopy2 = *cardHandCopyptr;
    Hand* cardHandCopy2ptr = &cardHandCopy2;
    cout << *cardHandptr;
    cout << *cardHandCopyptr;
    cout << *cardHandCopy2ptr;

    cardHandptr->play(6, cardDeckptr); //playing an individual card by position in hand
    cardHandptr->playAllCards(cardDeckptr);

};

int main() {
    CardsDriver();
}