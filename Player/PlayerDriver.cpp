//
// Created by sarah on 2021-09-28.
//
#include <iostream>
#include <vector>

using namespace std;

#include "PlayerDriver.h"


void PlayerDriver(){

    vector<Territory*> territories;
    territories.push_back(new Territory(99,"Lebanon",3));
    territories.push_back(new Territory(98,"Morocco",1));
    territories.push_back(new Territory(97,"Ireland",4));


    vector<Card*>cards;
    cards.push_back(new Card("bomb"));
    cards.push_back(new Card("reinforcement"));
    cards.push_back(new Card("blockade"));
    cards.push_back(new Card("airlift"));
    cards.push_back(new Card("diplomacy"));
    Hand * handOfCards=new Hand(cards);



    vector<Order*> orders;
    orders.push_back(new AirliftOrder());
    orders.push_back(new NegotiateOrder());
    OrdersList *ordersList = new OrdersList(orders);



    //  testing default constructor
    Player *player1 = new Player();
    cout << "\t\t\t***Testing the default Constructor***"<<endl;
    cout<< "This is the information about player No. 1"<<endl;
    cout<<*player1;
    cout<<endl;

    // testing parameterized constructor
    cout<< "\t\t\t***Testing the Parameterized Constructor***"<<endl;
    cout<< "This is the information about player No. 2"<<endl;
    Player *player2 = new Player("PlayerNO.2",territories);
    cout<<*player2;

    Territory *newTer = new Territory(98,"Morocco",1);
    player2->removeTerritory(newTer);
//
//    // testing copy constructor
//    cout<< "\t\t\t***Testing the copy Constructor***"<<endl;
//    cout<< "This is the information about player No. 3"<<endl;
//    Player *player3 = new Player(*player2);
//    cout<<*player3;
//    // adding a territory to original player (player 2)
//    cout<<"!!!Adding a territory to player No.2!!!"<<endl;
//    Territory *newTerritory =new Territory(25,"South Africa",3);
//    player2->addTerritory(newTerritory);
//    cout<< "This is the information about player No. 2"<<endl;
//    cout<<*player2;
//    cout<< "This is the information about player No. 3"<<endl;
//    cout<<*player3;
//
//    // testing all getters
//    // testing get  and set player's name functions
//    cout << "\t\t\t***Testing set and get function for player name***"<<endl;
//    cout<<"player no.1 name old name is "<< player1->getName()<<endl;
//    player1->setName("PlayerNO.1_newName");
//    string playerName =  player1->getName();
//    cout<<"Player new name is "<<playerName <<endl;
//    cout<<endl;
//
//    // testing get and set player's territories functions
//    cout << "\t\t\t***Testing set and get territories for player No.1 who had no territories***"<<endl;
//    player1->setTerritories(territories);
//    cout << "player no.1 territories are: "<<endl<<"\t";
//    vector<Territory*> playerTerritory = player1->getTerritories();
//    for (auto & territory : playerTerritory){
//        cout<<*territory<<"\t";
//    }
//    cout<<endl;
//    cout<<endl;
//
//    // testing get and set for player's hand of cards functions
//    cout << "\t\t\t***Testing set and get hand of cards for player No.1 who had no cards***"<<endl;
//    player1->setPlayerCards(handOfCards);
//    cout << "player no.1 's hand of cards is: "<<endl;
//    cout<<*player1->getPlayerCards()<<endl;
//    cout << endl;
//
//    //testing get and set for player's list of order unctions
//    cout << "\t\t\t***Testing set and get list of order for player No.1 who had no orders in his list***"<<endl;
//    player1->setPlayerOrdersList(ordersList);
//    cout << "player no.1 's orders list is: "<<endl;
//    cout<<player1->getPlayerOrdersList()<<endl;
//    cout<<endl;
//    cout<<endl;
//
//
//    // testing toDefend function
//    cout <<"\t\t\t***Testing toDefend Function***"<<endl;
//    cout << "The list of to be defended territories is:"<< endl<<"\t";
//    vector<Territory*> to_be_defended_territories = player1->toDefend();
//    for (auto & territory : to_be_defended_territories){
//        cout<<*territory<<"\t";
//    }
//    cout<<endl;
//    cout<<endl;
//
//
//    // testing toAttach function
//    cout <<"\t\t\t***Testing toAttack Function***"<<endl;
//    cout << "The list of to be attacked territories is:"<< endl<<"\t";
//    vector<Territory*> to_be_attacked_territories = player1->toAttack();
//    for (auto & territory : to_be_attacked_territories){
//        cout<<*territory<<"\t";
//    }
//    cout<<endl;
//    cout<<endl;
//
//
//    // testing issueOrder function
//    cout <<"\t\t\t***Testing issueOrder Function***"<<endl;
//    cout << "The list of player order is:"<< endl;
//    player1->issueOrder();
//    cout <<player1->getPlayerOrdersList()<<endl;
//
//    cout<<endl;
//
//
//    // clearing the heap
//    // deleting territories vector
//    delete newTerritory;
//    newTerritory = NULL;
//
//    for(auto territory :territories ){
//        delete territory;
//    }
//    territories.clear();
//
//    for(auto territory :playerTerritory){
//        delete territory;
//    }
//    playerTerritory.clear();
//
//    for(auto order : orders ){
//        delete order;
//    }
//    orders.clear();
//
//    for(auto card:cards ){
//        delete card;
//    }
//    cards.clear();
//
//    delete ordersList;
//    ordersList = NULL;
//
//    delete handOfCards;
//    handOfCards = NULL;
//
//    delete player1 ;
//    player1= NULL;
//
//    delete player2 ;
//    player2= NULL;
//
//    delete player3;
//    player3= NULL;
//
//
//
//
//
//
//
//
//
}
