//
// Created by Thong Tran on 2021-12-02.
//

#include <iostream>
#include "PSDriver.h"

void PSDriver()
{
    new GameEngine();
    std::cout << "\n~~~ Testing PlayerStrategyDriver ~~~\n" << std::endl;
    Player* humanPlayer = new Player("Human");
    Player* aggressivePlayer = new Player("Aggressive");

    humanPlayer->setStrategy(Human);
    aggressivePlayer->setStrategy(Aggressive);

    humanPlayer->setReinforcementPool(20);
    aggressivePlayer ->setReinforcementPool(20);

    humanPlayer->getPlayerCards()->drawFromDeck(GameEngine::deck);
    humanPlayer->getPlayerCards()->drawFromDeck(GameEngine::deck);
    humanPlayer->getPlayerCards()->drawFromDeck(GameEngine::deck);

/**
     *creating a continent object
     */
    Continent *continent = new Continent(9,"Africa",10);

    /**
     * creating a list of territories
     */
    vector<Territory*> territories;
    Territory* kenya = new Territory(99,"Kenya",3,continent);
    Territory* morocco = new Territory(98,"Morocco",1,continent);
    Territory* egypt = new Territory(97,"Egypt",4,continent);


    territories.push_back(kenya);
    territories.push_back(morocco);
    territories.push_back(egypt);

    /**
     * creating another list of territories
     */
    vector<Territory*> another_territories;
    vector<Territory*> another_territories2;
    Territory *china = new Territory(99,"China",3,continent);
    Territory* france = new Territory(97,"France",5,continent);
    Territory* germany = new Territory(97,"Germany",1,continent);
    Territory* greece = new Territory(97,"Greece",2,continent);
    Territory* austria = new Territory(97,"Austria",3,continent);
    another_territories.push_back(china);
    another_territories.push_back(france);
    another_territories.push_back(germany);
    another_territories.push_back(greece);
    another_territories2.push_back(austria);

    kenya->setAdjTerritories(another_territories.size(), &(*another_territories.begin()));
    morocco->setAdjTerritories(another_territories2.size(), &(*another_territories2.begin()));

//    another_territories.push_back(new Territory(98,"Japan",1,continent));
//    another_territories.push_back(new Territory(97,"India",4,continent));
    for (auto &territory : territories){
        territory->setOwner(humanPlayer);
    }
    for (auto &territory : another_territories){
        territory->setOwner(aggressivePlayer);
    }
    humanPlayer->setTerritories(territories);
    aggressivePlayer->setTerritories(another_territories);

    aggressivePlayer->getStrategy()->toAttack(humanPlayer);
    cout<<"Aggressive player attacked human player"<<endl;

    humanPlayer->getStrategy()->toDefend(aggressivePlayer);
    std::cout << "\n~~~ Human finished defending ~~~\n" << std::endl;


    humanPlayer->getStrategy()->toAttack(aggressivePlayer);
    cout<<"human attacked aggressive"<<endl;

    aggressivePlayer->getStrategy()->toDefend(humanPlayer);
    HumanPlayerStrategy humanPlayerStrategy;
    humanPlayer->issueOrder();

}