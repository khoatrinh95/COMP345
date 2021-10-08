//
// Created by Ricky Lam on 2021-09-27.
//

#include <iostream>
#include "Cards.h"

using namespace std;

void CardDriver(){
    //Testing card constructors and operators
    Card* bombCard = new Card("bomb");
    Card* bombCardCopy = new Card(*bombCard);

    delete bombCardCopy;
    bombCardCopy = nullptr;
    delete bombCard;
    bombCard = nullptr;

    //Testing card constructors and operators
    Deck* cardDeckptr = new Deck();
    cardDeckptr->addCard(new Card("bomb"));
    cardDeckptr->addCard(new Card("reinforcement"));
    cardDeckptr->addCard(new Card("blockade"));
    cardDeckptr->addCard(new Card("airlift"));
    cardDeckptr->addCard(new Card("diplomacy"));
    cardDeckptr->addCard(new Card("bomb"));
    cardDeckptr->addCard(new Card("reinforcement"));
    cardDeckptr->addCard(new Card("blockade"));
    cardDeckptr->addCard(new Card("airlift"));
    cardDeckptr->addCard(new Card("diplomacy"));
    cout << *cardDeckptr << endl;

    Deck* cardDeckCopyptr = new Deck(*cardDeckptr);
    cout << *cardDeckCopyptr << endl;

    delete cardDeckCopyptr;
    cardDeckCopyptr = nullptr;


    //Testing hand constructors and operators
    Hand* cardHandptr = new Hand();
    cout << *cardHandptr << endl;

    for (int i = 0; i < 6; i++){
        cardHandptr->drawFromDeck(cardDeckptr);
    }

    Hand* cardHandCopyptr = new Hand(*cardHandptr);
    cout << *cardHandCopyptr << endl;

    delete cardHandCopyptr;
    cardHandCopyptr = nullptr;

    cout << "============================================================================================" << endl;
    cout << "=Testing play() functionality: playing card, adding order to player, returning card to deck=" << endl;
    cout << "============================================================================================" << endl;

    Player* player1 = new Player();
    player1->setName("PlayerNO.1_newName");
    player1->setPlayerCards(cardHandptr);
    OrdersList *ordersList = new OrdersList();
    player1->setPlayerOrdersList(ordersList);

    cout << "Player1's order list before playing cards in their hand: " << endl;
    cout << *(player1->getPlayerOrdersList()) << endl;

    cout << *cardDeckptr;
    cout << *cardHandptr;

    cardHandptr->playOneCard(6, cardDeckptr, player1); //playing an individual card by position in hand

    cout << "Player1's order list after playing 1 card in their hand: " << endl;
    cout << *(player1->getPlayerOrdersList()) << endl;

    cardHandptr->playAllCards(cardDeckptr, player1);

    cout << "Player1's order list after playing cards in their hand: " << endl;
    cout << *(player1->getPlayerOrdersList()) << endl;

    cout << *cardDeckptr;
    cout << *cardHandptr;


    //cleaning up memory

    delete player1;
    player1 = nullptr;

    //delete ordersList; //this is already being deleted from deleting player1 on line 87
    ordersList = nullptr;

    //delete cardHandptr; //this is already being deleted from deleting player1 on line 87
    cardHandptr = nullptr;

    delete cardDeckptr;
    cardDeckptr = nullptr;

};

