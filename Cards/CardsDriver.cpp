//
// Created by Ricky Lam on 2021-09-27.
//

#include "CardsDriver.h"

void CardsDriver(){
    Card* bombCardptr = new Card("bomb");
    Card* reinforcementCardptr = new Card("reinforcement");
    Card* blockadeCardptr = new Card("blockade");
    Card* airliftCardptr = new Card("airlift");
    Card* diplomacyCardptr = new Card("diplomacy");

    //cout << bombCard << reinforcementCard << blockadeCard << airliftCard << diplomacyCard << endl;

    Card bombCard2(bombCardptr->getType());
    Card reinforcementCard2(reinforcementCardptr->getType());
    Card blockadeCard2(blockadeCardptr->getType());
    Card airliftCard2(airliftCardptr->getType());
    Card diplomacyCard2(diplomacyCardptr->getType());
    Card* bombCard2ptr = &bombCard2;
    Card* reinforcementCard2ptr = &reinforcementCard2;
    Card* blockadeCard2ptr = &blockadeCard2;
    Card* airliftCard2ptr = &airliftCard2;
    Card* diplomacyCard2ptr = &diplomacyCard2;

    //cout << bombCard2 << reinforcementCard2 << blockadeCard2 << airliftCard2 << diplomacyCard2 << endl;

    Deck* cardDeckptr = new Deck();
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

    cout << cardDeckptr->printDeck();

    Hand* cardHandptr = new Hand();

    for (int i = 0; i < 6; i++){
        cardHandptr->drawFromDeck(cardDeckptr);
    }
    cout << cardHandptr->printHand();
    cardHandptr->playAllCards(cardDeckptr);

}