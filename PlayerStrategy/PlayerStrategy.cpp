//
// Created by Khoa Trinh on 2021-11-21.
//

#include "PlayerStrategy.h"
#include "../Player/Player.h"
#include <math.h>
#include "../Orders/Orders.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <unordered_set>
//// THONG
//
//
namespace
{
    // Custom comparator to sort Territories by the number of armies and then by the number of adjacent enemy territories.
    bool compareTerritoriesByEnemiesAndArmies(Territory t1, Territory t2, Player owner)
    {
        if (t1.getNumberOfArmies() != t2.getNumberOfArmies())
        {
            return t1.getNumberOfArmies() > t2.getNumberOfArmies();
        }

        int t1EnemyNeighbors = 0;
        int i=0;
        for(; i<t1.getNumAdjTerritories();i++){
            if(t1.getAdjTerritories()[i]->getOwner()->getName()!=owner.getName()){
                t1EnemyNeighbors++;
            }
        }

        int t2EnemyNeighbors = 0;
        int j=0;
        for(; j<t2.getNumAdjTerritories();j++){
            if(t2.getAdjTerritories()[j]->getOwner()->getName()!=owner.getName()){
                t2EnemyNeighbors++;
            }
        }
        return t1EnemyNeighbors >= t2EnemyNeighbors;
    }
}

using namespace std;



// SARAH
/**
 * it orders the player list of territories according to number of armies
 * @param player
 * @return all territories belong to a player in ascending order according to the number of player
 */

vector<Territory*>  BenevolentPlayerStrategy::toDefend(Player *player) {
    vector<Territory*>toDefend_Territories;
    vector<Territory* > orderTerritories = player->getTerritories();
    sort(orderTerritories.begin(), orderTerritories.end(), [](const Territory* lhs, const Territory* rhs) {
        return lhs->getNumberOfArmies() < rhs->getNumberOfArmies();
    });
    for(auto &territory : orderTerritories){
        toDefend_Territories.push_back(territory);
    }
    return toDefend_Territories;
}
/**
 * the benevolent player does not attack any territories
 * @param player
 * @return empty vector of territories
 */
vector<Territory*>  BenevolentPlayerStrategy::toAttack(Player *player) {
    vector<Territory*>toAttack_Territories;
    return toAttack_Territories;
}

/**
 * issue order will go through all benevolent player territories to add armies to the the ones with least number of armies
 * @param player
 */
void BenevolentPlayerStrategy::issueOrder(Player *player)  {
    vector<Territory*>toDefend_Territories = toDefend(player);
//    for (auto &ter: toDefend_Territories){
//        cout<<*ter<<endl;
//    }
    // using all the reinforcement armies for territories that do not have enough armies to protect itself
    while (player->getReinforcementPool()>0){
        for (auto &territory : toDefend_Territories){
            int playerArmies = player->getReinforcementPool();
            if (playerArmies > 0 ) {
                int armies = ceil(double(playerArmies) / 3);
                DeployOrder *deployOrder = new DeployOrder(player, armies, territory);
                player->getPlayerOrdersList()->add(deployOrder);
                player->setReinforcementPool(playerArmies - armies);
            }
        }
    }
    int  j = toDefend_Territories.size()-1;

    for (auto &territory : toDefend_Territories){
        int i = 0;
        //  if there is no enough armies in the reinforcement pool use advance order from adjacent territories
        // to the territories with the lowest number of armies
        for(  ; i<territory->getNumAdjTerritories();i++){
            if (territory->getOwner() == territory->getAdjTerritories()[i]->getOwner() &&
             territory->getNumberOfArmies()<territory->getAdjTerritories()[i]->getNumberOfArmies()-8){
                AdvanceOrder *advanceOrder = new AdvanceOrder(player, 4, territory->getAdjTerritories()[i],
                                                              territory);
                player->getPlayerOrdersList()->add(advanceOrder);
                break;
            }
        }
        //  if there is no enough armies in the reinforcement pool use Card to issue order
        // to protect the territories with the lowest number of armies
        if (i == territory->getNumAdjTerritories() && territory->getNumberOfArmies()<4){
            for (auto &card: player->getPlayerCards()->getHand()) {
                if (card!= nullptr && card->getType() == "Airlift") {
                    card->useCardtoCreateOrder(player,2, toDefend_Territories.at(j),territory);
                    player->getPlayerCards()->removeCard(card);
                    j--;
                }
            }
        }
    }
}

void BenevolentPlayerStrategy::print(Player *player) {
    cout<< "Player "<<player->getName()<<"'s strategy is BENEVOLENT."<<endl;

}


vector<Territory*>  NeutralPlayerStrategy::toDefend(Player *player) {
    vector<Territory*>toDefend_Territories;
    return toDefend_Territories;
}

vector<Territory*>  NeutralPlayerStrategy::toAttack(Player *player) {
    vector<Territory*>toAttack_Territories;
    return toAttack_Territories;
}

void NeutralPlayerStrategy::issueOrder(Player *player)  {

}

void NeutralPlayerStrategy::print(Player *player) {
    cout<< "Player "<<player->getName()<<"'s strategy is NEUTRAL."<<endl;

}


vector<Territory *> HumanPlayerStrategy::toDefend(Player *player) {
    // Needs human to make an interaction
    return player->getTerritories();
}

vector<Territory *> HumanPlayerStrategy::toAttack(Player *player) {
    std::vector<Territory*> ownedTerritories = player->getTerritories();
    std::vector<Territory*> territoriesToAttack;
    std::unordered_set<Territory*> territoriesSeen;
    for (auto &territory : ownedTerritories) {
        for (int i = 0; i<territory->getNumAdjTerritories(); i++){

            bool isEnemyOwned = find(ownedTerritories.begin(), ownedTerritories.end(), territory->getAdjTerritories()[i]) == ownedTerritories.end();
            bool alreadySeen = territoriesSeen.find(territory->getAdjTerritories()[i]) != territoriesSeen.end();

            if (isEnemyOwned && !alreadySeen)
            {
                territoriesToAttack.push_back(territory->getAdjTerritories()[i]);
                territoriesSeen.insert(territory->getAdjTerritories()[i]);
            }
        }
    }
    return territoriesToAttack;
}



void HumanPlayerStrategy::issueOrder(Player *player)  {
    std::vector<Territory*> territoriesToAttack = toAttack(player);
    std::vector<Territory*> territoriesToDefend = toDefend(player);
    vector<Territory*> another_territories;
    Continent *continent = new Continent(9,"Africa",10);
    Territory *china = new Territory(99,"China",3,continent);
    vector<Territory*> territories;
    territories.push_back(new Territory(99,"Kenya",3,continent));
    territories.push_back(new Territory(98,"Morocco",1,continent));
    territories.push_back(new Territory(97,"Egypt",4,continent));
    china->setAdjTerritories(territories.size(), &(*territories.begin()));
    another_territories.push_back(china);
    another_territories.push_back(new Territory(98,"Japan",1,continent));
    another_territories.push_back(new Territory(97,"India",4,continent));

    string name1,name2;
    string strategy1,strategy2;
    cout<<"Enter name player 1: ";
    cin >> name1;
    Player* player1 = new Player(name1);
    cout<<"Enter name player 2: ";
    cin >>name2;
    Player* player2 = new Player(name2);
    cout<<"Enter a strategy for "<<player1->getName()<<"(Aggressive,"
                                                       "    Human,"
                                                       "    Neutral,"
                                                       "    Cheater,"
                                                       "    Benevolent: ";
    cin>>strategy1;
    if(strategy1=="Human") {
        player1->setStrategy(Human);
    }
    if(strategy1=="Aggressive"){
        player1->setStrategy(Aggressive);
    }
    cout<<"Enter a strategy for "<<player2->getName()<<"(Aggressive,"
                                                       "    Human,"
                                                       "    Neutral,"
                                                       "    Cheater,"
                                                       "    Benevolent: ";
    cin>>strategy2;
    if(strategy2=="Human") {
        player2->setStrategy(Human);
    }
    if(strategy2=="Aggressive"){
        player2->setStrategy(Aggressive);
    }
    int option;
    cout<<"Enter the option. There are 4 options: "<<endl;
    cout<<"1. "+player1->getName()+" attacks "+player2->getName()<<endl;
    cout<<"2. "+player1->getName()+" defends "+player2->getName()<<endl;
    cout<<"3. "+player2->getName()+" attacks "+player1->getName()<<endl;
    cout<<"4. "+player2->getName()+" defends "+player1->getName()<<endl;

    cout<<"Enter your option: ";
    cin>>option;
    cout<<endl;
    ;
    if(option == 1){
        player1->getStrategy()->toAttack(player2);
        std::cout << "\n"<<player1->getName()<<" attacked "<<player2->getName()<<"\n" << std::endl;
    }
    if(option == 2){
        player1->getStrategy()->toDefend(player2);
        std::cout << "\n"<<player1->getName()<<" defended "<<player2->getName()<<"\n" << std::endl;

    }
    if(option == 3){
        player2->getStrategy()->toAttack(player1);
        std::cout << "\n"<<player2->getName()<<" attacked "<<player1->getName()<<"\n" << std::endl;
    }
    if(option == 4){
        player2->getStrategy()->toDefend(player1);
        std::cout << "\n"<<player2->getName()<<" defended "<<player1->getName()<<"\n" << std::endl;
    }

    cout<<"This is the end of HumanPlayerStrategy IssueOrder "<<endl;

}

void HumanPlayerStrategy::print(Player *player) {
    cout<< "Player "<<player->getName()<<"'s strategy is HUMAN."<<endl;
}

vector<Territory *> AggressivePlayerStrategy::toDefend(Player *player) {
    std::vector<Territory*> territoriesToDefend = player->getTerritories();
    auto sortLambda = [](auto t1, auto t2){ return t1->getNumberOfArmies() > t2->getNumberOfArmies(); };
    sort(territoriesToDefend.begin(), territoriesToDefend.end(), sortLambda);
    return territoriesToDefend;
}

// return a vector that has the player's strongest territory
vector<Territory *> AggressivePlayerStrategy::toAttack(Player *player) {
    std::vector<Territory*> sources = toDefend(player);
    std::vector<Territory*> territoriesToAttack;
    std::unordered_set<Territory*> territoriesSeen;

    Territory* strongest;
    int strongestArmy = 0;
    for (const auto &territory : sources)
    {
        if (territory->getNumberOfArmies()>=strongestArmy){
            strongestArmy= territory->getNumberOfArmies();
            strongest = territory;
        }
    }
    territoriesToAttack.push_back(strongest);

    return territoriesToAttack;
}

void AggressivePlayerStrategy::issueOrder(Player *player)  {

    std::vector<Territory*> territoriesToAttack = toAttack(player);
    Territory* strongest = territoriesToAttack.at(0);

    for (int i =0; i<strongest->getNumAdjTerritories(); i++){
        int playerArmies = player->getReinforcementPool();
        if (playerArmies>3){
            int armies = ceil(double(playerArmies) / 3);
            AdvanceOrder* advanceOrder = new AdvanceOrder(player, armies, strongest, strongest->getAdjTerritories()[i]);
            player->getPlayerOrdersList()->add(advanceOrder);
            player->setReinforcementPool(playerArmies - armies);
        }
    }

//

}

void AggressivePlayerStrategy::print(Player *player) {
    cout<< "Player "<<player->getName()<<"'s strategy is AGGRESSIVE."<<endl;

}

// KHOA

/**
 * defend method does not do anything for Cheater as he only attacks
 * @param player
 * @return
 */
vector<Territory *> CheaterPlayerStrategy::toDefend(Player *player) {
    // do nothing as cheater does not defend
    return vector<Territory *>();
}

/**
 * attack method returns the list of all territories owned by this player
 * @param player
 * @return
 */
vector<Territory *> CheaterPlayerStrategy::toAttack(Player *player) {
    return player->getTerritories();
}

/**
 * issue order method will go through every adjacent territory and issue an Advance Order with 10x
 * the amount of army inside that territory -> guarantee conquer
 * @param player
 */
void CheaterPlayerStrategy::issueOrder(Player *player) {
    vector<Territory *> territories_of_this_player = toAttack(player);
    for (auto &territory : territories_of_this_player){
        Territory** adjTerritories = territory->getAdjTerritories();
        for (int i = 0; i<territory->getNumAdjTerritories(); i++){
            int armyToAttack = (adjTerritories[i]->getNumberOfArmies()*2);
            if (armyToAttack == 0 || armyToAttack >50) {
                armyToAttack =20;
            }
            AdvanceOrder *advanceOrder = new AdvanceOrder(player, armyToAttack, territory, adjTerritories[i]);
            player->getPlayerOrdersList()->add(advanceOrder);
            player->setReinforcementPool(player->getReinforcementPool()+armyToAttack);
        }
    }
}

void CheaterPlayerStrategy::print(Player *player) {
    cout<< "Player "<<player->getName()<<"'s strategy is CHEATER."<<endl;

}
