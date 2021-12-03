//
// Created by Khoa Trinh on 2021-11-21.
//

#include "PlayerStrategy.h"
#include "../Player/Player.h"
#include <math.h>
#include "../Orders/Orders.h"
#include <vector>
#include <algorithm>
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
    return vector<Territory *>();
}

vector<Territory *> HumanPlayerStrategy::toAttack(Player *player) {
    return vector<Territory *>();
}

void HumanPlayerStrategy::issueOrder(Player *player)  {

}

void HumanPlayerStrategy::print(Player *player) {
    cout<< "Player "<<player->getName()<<"'s strategy is HUMAN."<<endl;
}

vector<Territory *> AggressivePlayerStrategy::toDefend(Player *player) {
    return vector<Territory *>();
}

vector<Territory *> AggressivePlayerStrategy::toAttack(Player *player) {
    return vector<Territory *>();
}

void AggressivePlayerStrategy::issueOrder(Player *player)  {

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
            if (armyToAttack == 0 || armyToAttack > 50) {
                armyToAttack = 20;
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
