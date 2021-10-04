//
// Created by Ricky Lam on 2021-09-27.
//

#include <iostream>
#include "Cards.cpp"

using namespace std;

void CardsDriver(){
    //Testing card constructors and operators
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

    Card bombCard2(*bombCardptr);
    Card reinforcementCard2(*reinforcementCardptr);
    Card blockadeCard2(*blockadeCardptr);
    Card airliftCard2;
    airliftCard2 = airliftCard;
    Card diplomacyCard2;
    diplomacyCard2 = diplomacyCard;
    Card* bombCard2ptr = &bombCard2;
    Card* reinforcementCard2ptr = &reinforcementCard2;
    Card* blockadeCard2ptr = &blockadeCard2;
    Card* airliftCard2ptr = &airliftCard2;
    Card* diplomacyCard2ptr = &diplomacyCard2;


    //Testing card constructors and operators
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


    //Testing hand constructors and operators
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

    cout << "============================================================================================" << endl;
    cout << "=Testing play() functionality: playing card, adding order to player, returning card to deck=" << endl;
    cout << "============================================================================================" << endl;

    Player *player1 = new Player();
    player1->setName("PlayerNO.1_newName");
    player1->setPlayerCards(cardHandptr);
    OrdersList *ordersList = new OrdersList();
    player1->setPlayerOrdersList(ordersList);

    cout << "Player1's order list before playing cards in their hand: " << endl;
    cout << player1->getPlayerOrdersList() << endl;

    cout << *cardDeckptr;
    cout << *cardHandptr;

    cardHandptr->playOneCard(6, cardDeckptr, player1); //playing an individual card by position in hand
    cardHandptr->playAllCards(cardDeckptr, player1);

    cout << "Player1's order list after playing cards in their hand: " << endl;
    cout << player1->getPlayerOrdersList() << endl;

    cout << *cardDeckptr;
    cout << *cardHandptr;

    //cleaning up memory
    delete player1;
    player1 = NULL;
    delete ordersList;
    ordersList = NULL;

    delete cardHandptr;
    cardHandptr = NULL;
    delete cardHandCopyptr;
    cardHandCopyptr = NULL;
    delete cardHandCopy2ptr;
    cardHandCopy2ptr = NULL;

    delete cardDeckptr;
    delete cardDeckCopyptr;
    delete cardDeckCopy2ptr;
    cardDeckptr = NULL;
    cardDeckCopyptr = NULL;
    cardDeckCopy2ptr = NULL;

    delete bombCard2ptr;
    delete reinforcementCard2ptr;
    delete blockadeCard2ptr;
    delete airliftCard2ptr;
    delete diplomacyCard2ptr;
    bombCard2ptr = NULL;
    reinforcementCard2ptr = NULL;
    blockadeCard2ptr = NULL;
    airliftCard2ptr = NULL;
    diplomacyCard2ptr = NULL;

    delete bombCardptr;
    delete reinforcementCardptr;
    delete blockadeCardptr;
    delete airliftCardptr;
    delete diplomacyCardptr;
    bombCardptr = NULL;
    reinforcementCardptr = NULL;
    blockadeCardptr = NULL;
    airliftCardptr = NULL;
    diplomacyCardptr = NULL;
};

int main() {
    CardsDriver();
}