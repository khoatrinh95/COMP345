//
// Created by Khoa Trinh on 2021-11-21.
//

#include "PlayerStrategy.h"
#include "../Player/Player.h"
#include <math.h>
#include "../Orders/Orders.h"
#include "../GameEngine/GameEngine.h"
#include "../Player/Player.h"
#include <algorithm>
#include <math.h>
#include <time.h>
#include <unordered_set>
// THONG
/*
===================================
 Implementation for HumanPlayerStrategy class
===================================
 */

// Operator overloading
std::ostream &HumanPlayerStrategy::print_(std::ostream &output) const
{
    output << "[HumanPlayerStrategy]";
    return output;
}

// Return a pointer to a new instance of HumanPlayerStrategy.
PlayerStrategy* HumanPlayerStrategy::clone() const
{
    return new HumanPlayerStrategy();
}

// Return a list of territories to defend
std::vector<Territory*> HumanPlayerStrategy::toDefend(const Player* player) const
{
    return player->territories;
}

// Return a list of territories to attack
std::vector<Territory*> HumanPlayerStrategy::toAttack(const Player* player) const
{
    std::vector<Territory*> ownedTerritories = player->territories;
    std::vector<Territory*> territoriesToAttack;
    std::unordered_set<Territory*> territoriesSeen;
    Map* map = GameEngine::getMap();

    for (const auto &territory : ownedTerritories)
    {
        for (const auto &neighbor : map->getAdjacentTerritories(territory))
        {
            bool isEnemyOwned = find(ownedTerritories.begin(), ownedTerritories.end(), neighbor) == ownedTerritories.end();
            bool alreadySeen = territoriesSeen.find(neighbor) != territoriesSeen.end();

            if (isEnemyOwned && !alreadySeen)
            {
                territoriesToAttack.push_back(neighbor);
                territoriesSeen.insert(neighbor);
            }
        }
    }

    return territoriesToAttack;
}

// Issue an order based on user input
void HumanPlayerStrategy::issueOrder(Player* player)
{
    std::vector<Territory*> territoriesToAttack = toAttack(player);
    std::vector<Territory*> territoriesToDefend = toDefend(player);

    // Only allow deploy if the player still has reinforcements
    if (player->reinforcement_pool > 0)
    {
        deployReinforcements_(player, territoriesToDefend);
    }
    else
    {
        std::cout << "What would you like to do?" << std::endl;
        if (player->getOwnTerritoriesWithMovableArmies().size() > 0)
        {
            std::cout << "[A] Advance" << std::endl;
        }
        if (player->hand_->getSize() > 0)
        {
            std::cout << "[P] Play a card" << std::endl;
        }
        std::cout << "[X] Commit" << std::endl;

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
            else if (selection == "P")
            {
                playCard_(player, territoriesToDefend);
                break;
            }
            else if (selection == "X")
            {
                player->committed_ = true;
                break;
            }
        }
    }
}

// Deploy player's reinforcements to specified territory
void HumanPlayerStrategy::deployReinforcements_(Player* player, std::vector<Territory*> territoriesToDefend)
{
    std::cout << "You have " << player->reinforcement_pool << " reinforcements left." << std::endl;
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

        if (std::cin.fail() || selection < 1 || selection > player->reinforcement_pool)
        {
            std::cout << "Please enter a number between 1 and " << player->reinforcement_pool << ": " << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        armiesToDeploy = selection;
    }

    DeployOrder* order = new DeployOrder(player, armiesToDeploy, deployTarget);
    player->addOrder(order);
    deployTarget->addPendingIncomingArmies(armiesToDeploy);
    player->reinforcements_ -= armiesToDeploy;

    std::cout << "Issued: " << *order << std::endl << std::endl;
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

        if (std::cin.fail() || selection - 1 < 0 || selection - 1 >= possibleSources.size())
        {
            std::cout << "That was not a valid option. Please try again:" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        source = possibleSources.at(selection - 1);
    }

    // Display adjacent territories as either attackable or defendable
    Map* map = GameEngine::getMap();
    std::vector<Territory*> attackable;
    std::vector<Territory*> defendable;
    for (const auto &neighbor : map->getAdjacentTerritories(source))
    {
        if (find(territoriesToDefend.begin(), territoriesToDefend.end(), neighbor) != territoriesToDefend.end())
        {
            defendable.push_back(neighbor);
        }
        else
        {
            attackable.push_back(neighbor);
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
    player->addOrder(order);
    source->addPendingOutgoingArmies(armiesToMove);

    std::cout << "Issued: " << *order << std::endl << std::endl;
}

// Play a card from the player's hand
void HumanPlayerStrategy::playCard_(Player* player, std::vector<Territory*> territoriesToDefend)
{
    Hand* playerHand = player->hand_;

    std::cout << "\nWhich card would you like to play?" << std::endl;
    for (int i = 0; i < playerHand->size(); i++)
    {
        Card* card = playerHand->getCards().at(i);
        std::cout << "[" << i+1 << "] " << *card << std::endl;
    }

    Card* card = nullptr;
    std::cout << "\nEnter the card to play: ";
    while (card == nullptr)
    {
        int selection;
        std::cin >> selection;

        if (std::cin.fail() || selection - 1 < 0 || selection - 1 >= playerHand->size())
        {
            std::cout << "That was not a valid option. Please try again:" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        card = playerHand->removeCard(selection - 1);
    }

    Order* order = card->play();

    // Return the played card back to the deck
    card->setOwner(nullptr);
    GameEngine::getDeck()->addCard(card);

    if (order != nullptr)
    {
        player->addOrder(order);
        std::cout << "Issued: " << *order << std::endl << std::endl;
    }
    else if (player->reinforcements_ > 0)
    {
        deployReinforcements_(player, territoriesToDefend);
    }
}


// SARAH




vector<Territory*>  BenevolentPlayerStrategy::toDefend(Player *player) {
    return {};
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
    return {};
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


vector<Territory *> AggressivePlayerStrategy::toDefend(Player *player) {
    return vector<Territory *>();
}

vector<Territory *> AggressivePlayerStrategy::toAttack(Player *player) {
    return vector<Territory *>();
}

void AggressivePlayerStrategy::issueOrder(Player *player)  {

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
/**
 * attack method returns the list of all territories owned by this player
 * @param player
 * @return
 */
vector<Territory *> CheaterPlayerStrategy::toAttack(Player *player) {
    return player->getTerritories();
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
            int armyToAttack = adjTerritories[i]->getNumberOfArmies()*10;
            AdvanceOrder *advanceOrder = new AdvanceOrder(player, armyToAttack, territory, adjTerritories[i]);
            player->getPlayerOrdersList()->add(advanceOrder);
            player->setReinforcementPool(player->getReinforcementPool()+armyToAttack);
        }
    }