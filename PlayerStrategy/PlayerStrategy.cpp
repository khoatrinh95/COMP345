//
// Created by Khoa Trinh on 2021-11-21.
//

#include "PlayerStrategy.h"
#include "../Player/Player.h"
#include <math.h>
#include "../Orders/Orders.h"




// SARAH




vector<Territory*>  BenevolentPlayerStrategy::toDefend(Player *player) {
//    vector<Territory*>toDefend_Territories;
//    vector<Territory* > orderTerritories = getPlayer()->getTerritories();
//    sort(orderTerritories.begin(), orderTerritories.end(), [](const Territory& lhs, const Territory& rhs) {
//        return lhs.getNumberOfArmies() < rhs.getNumberOfArmies();
//    });
//    for(auto &territory : orderTerritories){
//        toDefend_Territories.push_back(territory);
//    }
//    return toDefend_Territories;
}

vector<Territory*>  BenevolentPlayerStrategy::toAttack(Player *player) {
//    vector<Territory*>toAttack_Territories;
//    return toAttack_Territories;
}

void BenevolentPlayerStrategy::issueOrder(Player *player)  {
//    vector<Territory*>toDefend_Territories = toDefend();
//    // using all the reinforcement armies for territories that do not have enough armies to protect itself
//    while (getPlayer()->getReinforcementPool()>0){
//        for (auto &territory : toDefend_Territories){
//            int playerArmies = this->getPlayer()->getReinforcementPool();
//            if (playerArmies > 0 && territory->getNumberOfArmies()<5) {
//                int armies = ceil(double(playerArmies) / 3);
//                DeployOrder *deployOrder = new DeployOrder(getPlayer(), armies, territory);
//                getPlayer()->getPlayerOrdersList()->add(deployOrder);
//                getPlayer()->setReinforcementPool(playerArmies - armies);
//            }
//        }
//    }
//    int  j = toDefend_Territories.size()-1;
//
//    for (auto &territory : toDefend_Territories){
//        int i = 0;
//        //  if there is no enough armies in the reinforcement pool use advance order from adjacent territories
//        // to the territories with the lowest number of armies
//        for(  ; i<territory->getNumAdjTerritories();i++){
//            if (territory->getOwner() == territory->getAdjTerritories()[i]->getOwner() &&
//             territory->getNumberOfArmies()<territory->getAdjTerritories()[i]->getNumberOfArmies()-8){
//                AdvanceOrder *advanceOrder = new AdvanceOrder(getPlayer(), 4, territory->getAdjTerritories()[i],
//                                                              territory);
//                getPlayer()->getPlayerOrdersList()->add(advanceOrder);
//                break;
//            }
//        }
//        //  if there is no enough armies in the reinforcement pool use Card to issue order
//        // to protect the territories with the lowest number of armies
//        if (i == territory->getNumAdjTerritories() && territory->getNumberOfArmies()<4){
//            for (auto &card: getPlayer()->getPlayerCards()->getHand()) {
//                if (card->getType() == "Airlift") {
//                    AirliftOrder *airliftOrder = new AirliftOrder(getPlayer(), 2,toDefend_Territories.at(j) ,territory);
//                    getPlayer()->getPlayerOrdersList()->add(airliftOrder);
//                    j--;
//                }
//            }
//        }
//    }
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






vector<Territory *> HumanPlayerStrategy::toDefend(Player *player) {
    return vector<Territory *>();
}

vector<Territory *> HumanPlayerStrategy::toAttack(Player *player) {
    return vector<Territory *>();
}

void HumanPlayerStrategy::issueOrder(Player *player)  {

}

vector<Territory *> AggressivePlayerStrategy::toDefend(Player *player) {
    return vector<Territory *>();
}

vector<Territory *> AggressivePlayerStrategy::toAttack(Player *player) {
    return vector<Territory *>();
}

void AggressivePlayerStrategy::issueOrder(Player *player)  {

}

vector<Territory *> CheaterPlayerStrategy::toDefend(Player *player) {
    return vector<Territory *>();
}

vector<Territory *> CheaterPlayerStrategy::toAttack(Player *player) {
    return vector<Territory *>();
}

void CheaterPlayerStrategy::issueOrder(Player *player) {

}
