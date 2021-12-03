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

// Deploy player's reinforcements to specified territory
void HumanPlayerStrategy::deployReinforcements_(Player* player, std::vector<Territory*> territoriesToDefend)
{
    std::cout << "You have " << player->getReinforcementPool() << " reinforcements left." << std::endl;
    std::cout << "\nWhere would you like to deploy to?" << std::endl;
    for (int i = 0; i < territoriesToDefend.size(); i++)
    {
        Territory* territory = territoriesToDefend.at(i);
        std::cout << "[" << i+1 << "] " << territory->getName() << " (" << territory->getNumberOfArmies() << " present, " << territory->getPendingIncomingArmies() << " pending)" << std::endl;
    }

    Territory* deployTarget = nullptr;
    std::cout << "\nEnter the territory to deploy to: ";
    while (deployTarget == nullptr)
    {
        int selection;
        std::cin >> selection;

        if (std::cin.fail() || selection - 1 < 0 || selection - 1 >= territoriesToDefend.size())
        {
            std::cout << "That was not a valid option. Please try again:" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        deployTarget = territoriesToDefend.at(selection - 1);
    }

    int armiesToDeploy = 0;
    std::cout << "How many reinforcements do you want to deploy? ";
    while (armiesToDeploy == 0)
    {
        int selection;
        std::cin >> selection;

        if (std::cin.fail() || selection < 1 || selection > player->getReinforcementPool())
        {
            std::cout << "Please enter a number between 1 and " << player->getReinforcementPool() << ": " << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        armiesToDeploy = selection;
    }

    DeployOrder* order = new DeployOrder(player, armiesToDeploy, deployTarget);
    player->getPlayerOrdersList()->add(order);
    deployTarget->addPendingIncomingArmies(armiesToDeploy);
    player->setReinforcementPool(armiesToDeploy);

    std::cout << "Issued: " << *order << std::endl << std::endl;
}
/**
 * An airlift order tells a certain number of armies taken from a source territory to be moved to a target territory,
 * the source territory being owned by the player issuing the order
 * @param player
 * @param territoriesToDefend
 */
void HumanPlayerStrategy::issueAirLift_(Player* player, std::vector<Territory*> territoriesToDefend){

}
// Issue an advance order to either fortify or attack a territory
void HumanPlayerStrategy::issueAdvance_(Player* player, std::vector<Territory*> territoriesToDefend)
{
    std::vector<Territory*> possibleSources = player->getOwnTerritoriesWithMovableArmies();

    std::cout << "\nWhich territory would you like to advance from?" << std::endl;
    for (int i = 0; i < possibleSources.size(); i++)
    {
        Territory* territory = possibleSources.at(i);
        std::cout << "[" << i+1 << "] " << territory->getName() << " (" << territory->getNumberOfMovableArmies() << " armies available)" << std::endl;
    }

    Territory* source = nullptr;
    std::cout << "\nEnter the territory to advance from: ";
    while (source == nullptr)
    {
        int selection;
        std::cin >> selection;

        if (std::cin.fail() || selection - 1 < 0 )
        {
            std::cout << "That was not a valid option. Please try again:" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        source = possibleSources.at(selection - 1);
    }

    // Display adjacent territories as either attackable or defendable
    std::vector<Territory*> attackable;
    std::vector<Territory*> defendable;
    int index=0;
    for(;index<source->getNumAdjTerritories();index++){
        Territory* territory = source->getAdjTerritories()[index];
        if (find(territoriesToDefend.begin(), territoriesToDefend.end(), territory) != territoriesToDefend.end())
        {
            defendable.push_back(source->getAdjTerritories()[index]);
        }
        else
        {
            attackable.push_back(source->getAdjTerritories()[index]);
        }
    }


    int i = 0;
    std::cout << "\nWhich territory would you like to advance to?" << std::endl;
    if (!defendable.empty())
    {
        std::cout << "~~~ To Defend ~~~" << std::endl;
        for (; i < defendable.size(); i++)
        {
            Territory* territory = defendable.at(i);
            std::cout << "[" << i+1 << "] " << territory->getName() << " (" << territory->getNumberOfArmies() << " armies present)" << std::endl;
        }
        std::cout << std::endl;
    }
    if (!attackable.empty())
    {
        std::cout << "~~~ To Attack ~~~" << std::endl;
        for (; i < defendable.size() + attackable.size(); i++)
        {
            int idx = i - defendable.size();
            Territory* territory = attackable.at(idx);
            std::cout << "[" << i+1 << "] " << territory->getName() << " (" << territory->getNumberOfArmies() << " armies present)" << std::endl;
        }
    }

    Territory* destination = nullptr;
    std::cout << "\nEnter the territory to advance to: ";
    while (destination == nullptr)
    {
        int selection;
        std::cin >> selection;

        if (std::cin.fail() || selection - 1 < 0 || selection - 1 >= defendable.size() + attackable.size())
        {
            std::cout << "That was not a valid option. Please try again:" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (selection <= defendable.size())
        {
            destination = defendable.at(selection - 1);
        }
        else
        {
            destination = attackable.at(selection - 1 - defendable.size());
        }
    }

    int armiesToMove = 0;
    int movableArmies = source->getNumberOfMovableArmies();
    std::cout << "How many armies do you want to move? ";
    while (armiesToMove == 0)
    {
        int selection;
        std::cin >> selection;

        if (std::cin.fail() || selection < 1 || selection > movableArmies)
        {
            std::cout << "Please enter a number between 1 and " << movableArmies << ": " << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        armiesToMove = selection;
    }

    AdvanceOrder* order = new AdvanceOrder(player, armiesToMove, source, destination);
    player->getPlayerOrdersList()->add(order);
    source->addPendingOutgoingArmies(armiesToMove);

    std::cout << "Issued: " << *order << std::endl << std::endl;
    order->execute();
}

Territory* showOwnedTerritoriesForInput(Player* player){
    std::vector<Territory*> possibleSources = player->getOwnTerritoriesWithMovableArmies();

    std::cout << "\nWhich territory would you like to play the card with?" << std::endl;
    for (int i = 0; i < possibleSources.size(); i++)
    {
        Territory* territory = possibleSources.at(i);
        std::cout << "[" << i+1 << "] " << territory->getName() << " (" << territory->getNumberOfMovableArmies() << " armies available)" << std::endl;
    }

    Territory* source = nullptr;
    std::cout << "\nEnter the territory to advance from: ";
    while (source == nullptr)
    {
        int selection;
        std::cin >> selection;

        if (std::cin.fail() || selection - 1 < 0 )
        {
            std::cout << "That was not a valid option. Please try again:" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        source = possibleSources.at(selection - 1);
    }
    return source;
}

Territory* showAdjTerritoriesForInput(Territory* source){
    // Display adjacent territories as either attackable or defendable
    std::vector<Territory*> adjTerritories;
    std::vector<Territory*> defendable;
    int index=0;
    for(;index<source->getNumAdjTerritories();index++){
        Territory* territory = source->getAdjTerritories()[index];
        adjTerritories.push_back(source->getAdjTerritories()[index]);
    }


    int i = 0;
    std::cout << "\nWhich territory would you like be the target?" << std::endl;
    if (!adjTerritories.empty())
    {
        std::cout << "~~~ To Attack ~~~" << std::endl;
        for (; i < defendable.size() + adjTerritories.size(); i++)
        {
            int idx = i - defendable.size();
            Territory* territory = adjTerritories.at(idx);
            std::cout << "[" << i+1 << "] " << territory->getName() << " (" << territory->getNumberOfArmies() << " armies present)" << std::endl;
        }
    }

    Territory* destination = nullptr;
    std::cout << "\nEnter the territory to advance to: ";
    while (destination == nullptr)
    {
        int selection;
        std::cin >> selection;

        if (std::cin.fail() || selection - 1 < 0 || selection - 1 >= defendable.size() + adjTerritories.size())
        {
            std::cout << "That was not a valid option. Please try again:" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (selection <= defendable.size())
        {
            destination = defendable.at(selection - 1);
        }
        else
        {
            destination = adjTerritories.at(selection - 1 - defendable.size());
        }
    }
    return destination;
}

int takeInputForArmies(Territory* source){
    int armiesToMove = 0;
    int movableArmies = source->getNumberOfMovableArmies();
    std::cout << "How many armies do you want to move? ";
    while (armiesToMove == 0)
    {
        int selection;
        std::cin >> selection;

        if (std::cin.fail() || selection < 1 || selection > movableArmies)
        {
            std::cout << "Please enter a number between 1 and " << movableArmies << ": " << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        armiesToMove = selection;
    }
    return armiesToMove;
}
//// Helper method to play a random Card from the Player's hand, if any.
//// Returns `true` if the Player has no more cards to play/no new order was issued.
//// Returns `false` if there was an order issued.
bool HumanPlayerStrategy::playCard_(Player* player)
{
    Territory* source = showOwnedTerritoriesForInput(player);
    Territory* destination = showAdjTerritoriesForInput(source);
    int army = takeInputForArmies(source);
    cout << "Playing a random card from hand" <<endl;



        player->getPlayerCards()->getHand().at(0)->useCardtoCreateOrder(player , army, source, destination);
        OrdersList* ordersList = player->getPlayerOrdersList();
        for (auto &order: ordersList->getOrders()){
            order->execute();
        }
        cout << "RESULT: " << endl;
        cout << source->getName() << " : " << source->getNumberOfArmies() << " armies" <<endl;
        cout << destination->getName() << " : " << destination->getNumberOfArmies() << " armies" <<endl << endl;



    return false;
}
void HumanPlayerStrategy::issueOrder(Player *player)  {
    std::vector<Territory*> territoriesToAttack = toAttack(player);
    std::vector<Territory*> territoriesToDefend = toDefend(player);

    cout<<"There are 4 options: "<<endl;

        std::cout << "What would you like to do?" << std::endl;
        if (player->getOwnTerritoriesWithMovableArmies().size() > 0)
        {
            std::cout << "[A] Advance" << std::endl;
            std::cout << "[D] Deploy" << std::endl;
            std::cout << "[C] Play a card" << std::endl;
        }

        while (true)
        {
            std::string selection;
            std::cin >> selection;
            std::transform(selection.begin(), selection.end(), selection.begin(), ::toupper);

            if (selection == "A")
            {
                issueAdvance_(player, territoriesToDefend);
                break;
            }
            else if (selection == "D")
            {
                deployReinforcements_(player, territoriesToDefend);
                break;
            }
            else if (selection == "C")
            {
                playCard_(player);
                break;
            }else {
                cout << "Invalid choice" << endl;
            }
        }
//    }
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
