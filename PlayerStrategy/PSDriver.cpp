//
// Created by Thong Tran on 2021-12-02.
//

#include <iostream>
#include "PSDriver.h"

void PSDriver()
{
    std::cout << "\n~~~ Testing PlayerStrategyDriver ~~~\n" << std::endl;
    Player* humanPlayer = new Player("Human");
    Player* aggressivePlayer = new Player("Aggressive");

    humanPlayer->setStrategy(Human);
    aggressivePlayer->setStrategy(Aggressive);
/**
     *creating a continent object
     */
    Continent *continent = new Continent(9,"Africa",10);

    /**
     * creating a list of territories
     */
    vector<Territory*> territories;
    territories.push_back(new Territory(99,"Kenya",3,continent));
    territories.push_back(new Territory(98,"Morocco",1,continent));
    territories.push_back(new Territory(97,"Egypt",4,continent));

    /**
     * creating another list of territories
     */
    vector<Territory*> another_territories;
    Territory *china = new Territory(99,"China",3,continent);
    china->setAdjTerritories(territories.size(), &(*territories.begin()));
    another_territories.push_back(china);
    another_territories.push_back(new Territory(98,"Japan",1,continent));
    another_territories.push_back(new Territory(97,"India",4,continent));

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