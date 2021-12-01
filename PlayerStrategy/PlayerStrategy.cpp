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


namespace
{
    // Custom comparator to sort Territories by the number of armies and then by the number of adjacent enemy territories.
    bool compareTerritoriesByEnemiesAndArmies(Territory* t1, Territory* t2, const Player* owner)
    {
        Map* map = GameEngine::getMap();
        int t1EnemyNeighbors = 0;
        for (const auto &territory : map->getAdjacentTerritories(t1))
        {
            if (territory->getOwner() != owner)
            {
                t1EnemyNeighbors++;
            }
        }

        int t2EnemyNeighbors = 0;
        for (const auto &territory : map->getAdjacentTerritories(t2))
        {
            if (territory->getOwner() != owner)
            {
                t2EnemyNeighbors++;
            }
        }

        if (t1->getNumberOfArmies() != t2->getNumberOfArmies())
        {
            return t1->getNumberOfArmies() > t2->getNumberOfArmies();
        }

        return t1EnemyNeighbors > t2EnemyNeighbors;
    }
}

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
    return player->getTerritories();
}

// Return a list of territories to attack
std::vector<Territory*> HumanPlayerStrategy::toAttack(const Player* player) const
{
    std::vector<Territory*> ownedTerritories = player->getTerritories();
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
    if (player->getReinforcementPool() > 0)
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
    player->getPlayerOrdersList()->add(order);
    source->addPendingOutgoingArmies(armiesToMove);

    std::cout << "Issued: " << *order << std::endl << std::endl;
}

// Play a card from the player's hand
void HumanPlayerStrategy::playCard_(Player* player, std::vector<Territory*> territoriesToDefend)
{
    Hand* playerHand = player->hand_;

    std::cout << "\nWhich card would you like to play?" << std::endl;
    for (int i = 0; i < playerHand->getSize(); i++)
    {
        Card* card = playerHand->getCards().at(i);              //TODO
        std::cout << "[" << i+1 << "] " << *card << std::endl;
    }

    Card* card = nullptr;
    std::cout << "\nEnter the card to play: ";
    while (card == nullptr)
    {
        int selection;
        std::cin >> selection;

        if (std::cin.fail() || selection - 1 < 0 || selection - 1 >= playerHand->getSize())
        {
            std::cout << "That was not a valid option. Please try again:" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        card = playerHand->removeCard(selection - 1);               //TODO    //I need removeCardByPosition
    }

    Order* order = card->play();

    // Return the played card back to the deck
    card->setOwner(nullptr);                                        //TODO:
    GameEngine::getDeck()->addCard(card);

    if (order != nullptr)
    {
        player->getPlayerOrdersList()->add(order);
        std::cout << "Issued: " << *order << std::endl << std::endl;
    }
    else if (player->getReinforcementPool() > 0)
    {
        deployReinforcements_(player, territoriesToDefend);
    }
}


/*
===================================
 Implementation for PlayerStrategy class
===================================
 */

// Operator overloading
std::ostream &operator<<(std::ostream &output, const PlayerStrategy &strategy)
{
    return strategy.print_(output);
}



/*
===================================
 Implementation for AggressivePlayerStrategy class
===================================
 */

// Operator overloading
std::ostream &AggressivePlayerStrategy::print_(std::ostream &output) const
{
    output << "[AggressivePlayerStrategy]";
    return output;
}

// Return a pointer to a new instance of AggressivePlayerStrategy.
PlayerStrategy* AggressivePlayerStrategy::clone() const
{
    return new AggressivePlayerStrategy();
}

// Return a list of territories to defend
std::vector<Territory*> AggressivePlayerStrategy::toDefend(const Player* player) const
{
    std::vector<Territory*> territoriesToDefend = player->getTerritories();
    auto sortLambda = [](auto t1, auto t2){ return t1->getNumberOfArmies() > t2->getNumberOfArmies(); };
    sort(territoriesToDefend.begin(), territoriesToDefend.end(), sortLambda);
    return territoriesToDefend;
}

// Return a list of territories to attack
std::vector<Territory*> AggressivePlayerStrategy::toAttack(const Player* player) const
{
    std::vector<Territory*> sources = toDefend(player);
    std::vector<Territory*> territoriesToAttack;
    std::unordered_set<Territory*> territoriesSeen;
    Map* map = GameEngine::getMap();

    for (const auto &territory : sources)
    {
        std::vector<Territory*> adjacentTerritories = map->getAdjacentTerritories(territory);
        auto sortLambda = [&player](auto t1, auto t2){ return compareTerritoriesByEnemiesAndArmies(t1, t2, player); };
        sort(adjacentTerritories.begin(), adjacentTerritories.end(), sortLambda);

        for (const auto &neighbor : adjacentTerritories)
        {
            bool isEnemyOwned = find(sources.begin(), sources.end(), neighbor) == sources.end();
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

// Issue an order to either:
// - Deploy to strongest territory
// - Play a card from the hand
// - Attack an enemy territory with the strongest territory
// - Advance all armies from the strongest territory to another territory (if surrounded by fristd::endly territories)
void AggressivePlayerStrategy::issueOrder(Player* player)
{
    std::vector<Territory*> territoriesToAttack = toAttack(player);
    std::vector<Territory*> territoriesToDefend = toDefend(player);

    bool finishedDeploying = deployToTopTerritory_(player, territoriesToDefend);
    if (finishedDeploying)
    {
        bool finishedPlayingCards = playCard_(player, territoriesToDefend);
        if (finishedPlayingCards)
        {
            bool finishedAttacking = attackFromTopTerritory_(player, territoriesToDefend.front(), territoriesToAttack);
            if (finishedAttacking)
            {
                bool finishedIssuingOrders = advanceToRandomTerritory_(player, territoriesToDefend);
                player->committed_ = finishedIssuingOrders;
            }
        }

    }
}

// Deploy all reinforcements to the strongest territory (the one with the most armies already present).
// Returns `true` if finished deploying/no new order was issued.
// Returns `false` if there was an order issued.
bool AggressivePlayerStrategy::deployToTopTerritory_(Player* player, std::vector<Territory*> territoriesToDefend)
{
    if (player->getReinforcementPool() == 0)
    {
        return true;
    }

    Territory* topTerritory = territoriesToDefend.front();
    DeployOrder* order = new DeployOrder(player, player->getReinforcementPool(), topTerritory);
    player->getPlayerOrdersList()->add(order);
    topTerritory->addPendingIncomingArmies(player->getReinforcementPool());
    player->setReinforcementPool(0);

    std::cout << "Issued: " << *order << std::endl;
    return false;
}

// Advance all armies from strongest territory to an enemy territory.
// Returns `true` if finished attacking or has no one to attack/no new order was issued.
// Returns `false` if there was an order issued.
bool AggressivePlayerStrategy::attackFromTopTerritory_(Player* player, Territory* attackFrom, std::vector<Territory*> territoriesToAttack)
{
    Map* map = GameEngine::getMap();
    int movableArmies = attackFrom->getNumberOfMovableArmies();

    if (movableArmies > 0)
    {
        for (const auto &territory : map->getAdjacentTerritories(attackFrom))
        {
            bool isEnemyTerritory = find(territoriesToAttack.begin(), territoriesToAttack.end(), territory) != territoriesToAttack.end();
            bool alreadyAdvancedToTerritory = player->advancePairingExists_(attackFrom, territory);

            if (isEnemyTerritory && !alreadyAdvancedToTerritory)
            {
                AdvanceOrder* order = new AdvanceOrder(player, movableArmies, attackFrom, territory);
                player->getPlayerOrdersList()->add(order);
                attackFrom->addPendingOutgoingArmies(movableArmies);
                player->issuedDeploymentsAndAdvancements_[attackFrom].push_back(territory);             //TODO

                std::cout << "Issued: " << *order << std::endl;
                return false;
            }
        }
    }

    return true;
}

// Advance armies from the top/strongest territory to another adjacent fristd::endly territory.
// This will ensure that the player doesn't get stuck deploying to the top territory indefinitely
// when the territory doesn't have any surrounding enemy territories to attack.
// Returns `true` if finished advanstd::cing/no new order was issued.
// Returns `false` if there was an order issued.
bool AggressivePlayerStrategy::advanceToRandomTerritory_(Player* player, std::vector<Territory*> territoriesToDefend)
{
    Territory* topTerritory = territoriesToDefend.front();
    int movableArmies = topTerritory->getNumberOfMovableArmies();

    // If the player hasn't already moved all the armies to attack an enemy, move to another fristd::endly territory
    if (movableArmies > 0)
    {
        std::vector<Territory*> adjacentTerritories = GameEngine::getMap()->getAdjacentTerritories(topTerritory);

        // Pick a random destination
        srand(time(nullptr));
        int randomIndex = rand() % adjacentTerritories.size();
        Territory* destination = adjacentTerritories.at(randomIndex);

        AdvanceOrder* order = new AdvanceOrder(player, movableArmies, topTerritory, destination);
        player->getPlayerOrdersList()->add(order);
        topTerritory->addPendingOutgoingArmies(movableArmies);
        player->issuedDeploymentsAndAdvancements_[topTerritory].push_back(destination);                 //TODO

        std::cout << "Issued: " << *order << std::endl;
        return false;
    }

    return true;
}

// Helper method to play a random Card from the Player's hand, if any.
// Returns `true` if the Player has no more cards to play/no new order was issued.
// Returns `false` if there was an order issued.
bool AggressivePlayerStrategy::playCard_(Player* player, std::vector<Territory*> territoriesToDefend)
{
    Hand* playerHand = player->hand_;
    if (playerHand->getSize() == 0)
    {
        return true;
    }

    // Play a random card from hand
    int randomCardIndex = rand() % playerHand->getSize();
    Card* card = playerHand->removeCard(randomCardIndex);       //TODO
    Order* order = card->play();
    std::cout << "Played: " << *card << std::endl;

    // Return the played card back to the deck
    card->setOwner(nullptr);                                     //TODO
    GameEngine::getDeck()->addCard(card);

    if (order != nullptr)
    {
        player->getPlayerOrdersList()->add(order);
        std::cout << "Issued: " << *order << std::endl;
    }
    else if (player->getReinforcementPool() > 0)
    {
        // Reinforcement card played: deploy the additional reinforcements
        deployToTopTerritory_(player, territoriesToDefend);
    }

    return false;
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
            int armyToAttack = adjTerritories[i]->getNumberOfArmies()*10;
            AdvanceOrder *advanceOrder = new AdvanceOrder(player, armyToAttack, territory, adjTerritories[i]);
            player->getPlayerOrdersList()->add(advanceOrder);
            player->setReinforcementPool(player->getReinforcementPool()+armyToAttack);
        }
    }
}