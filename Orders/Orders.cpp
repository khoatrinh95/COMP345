#include "../GameEngine/GameEngine.h"
#include "../Orders/Orders.h"
#include <iterator>
#include <math.h>
#include <algorithm>

namespace {
    // Orders are sorted by priority using a custom comparator.
    bool compareTwoOrders(Order *order1, Order *order2) {
        return order1->getPriority() < order2->getPriority();
    }

/**
 * This function check if a region is able to be attacked by a specific player
 * @param attacker
 * @param target
 * @return true if the attacker already had the target territory
 * or check if there is any diplomacy relations between the attacker and the territory target
 */
    bool checkIfPossibleToAttack(Player *attacker, Territory *target) {
        Player *ownerOfTarget = target->getOwner();
        std::vector<Player *> relations = attacker->getRelations();
        bool relationsWithHostOfTarget = find(relations.begin(), relations.end(), ownerOfTarget) != relations.end();

        if (relationsWithHostOfTarget) {
            std::cout << attacker->getName() << " and " << ownerOfTarget->getName()
                      << " cannot attack each other for the rest of this turn. ";
        }

        return attacker == ownerOfTarget || !relationsWithHostOfTarget;
    }
}


/**
 * Implementing Order classes
 */

// Constructors
Order::Order() : issuer_(nullptr), priority_(4) {}

Order::Order(Player *issuer, int priority) : issuer_(issuer), priority_(priority) {}

Order::Order(const Order &order) : issuer_(order.issuer_), priority_(order.priority_) {}

// Destructor
Order::~Order() {}

// Operator overloading
const Order &Order::operator=(const Order &order) {
    if (this != &order) {
        issuer_ = order.issuer_;
        priority_ = order.priority_;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &output, const Order &order) {
    return order.print_(output);
}

void Order::execute() {
    execute_();
    notify();
}

// Get order priority
int Order::getPriority() const {
    return priority_;
}

// Reset the pre-orders-execution game state to the state it was in before the order was placed.
// If there is no meta-state to reset, the default behaviour is to do nothing.

void Order::undo_() {}


/**
 * Implementing OrdersList classes
 */

// Constructors
OrdersList::OrdersList() {}

OrdersList::OrdersList(const OrdersList &orders) {
    for (const auto &order: orders.orders_) {
        orders_.push_back(order->clone());
    }
}

OrdersList::OrdersList(vector<Order *> orders) {
    this->orders_ = orders;
}

// Destructor
OrdersList::~OrdersList() {
    for (const auto &order: orders_) {
        delete order;
    }
    orders_.clear();
}

const OrdersList &OrdersList::operator=(const OrdersList &orders) {
    if (this != &orders) {
        setOrders(orders.orders_);
    }
    return *this;
}

std::ostream &operator<<(std::ostream &output, const OrdersList &orders) {
    output << "[Orders List] Size=" << orders.size() << endl;
    for (auto order: orders.getOrders()) {
        output << *order << endl;
    }
    return output;
}

// Getter and setter
std::vector<Order *> OrdersList::getOrders() const {
    return orders_;
}

void OrdersList::setOrders(std::vector<Order *> orders) {
    for (const auto &order: orders_) {
        delete order;
    }
    orders_.clear();

    for (const auto &order: orders) {
        orders_.push_back(order->clone());
    }
}

// Pop the first order in the OrderList according to priority
Order *OrdersList::popTopOrder() {
    Order *topOrder = peek();
    orders_.erase(orders_.begin());

    return topOrder;
}

// Get the first order in the OrderList according to priority without removing it
Order *OrdersList::peek() {
    if (orders_.empty()) {
        return nullptr;
    }

    sort(orders_.begin(), orders_.end(), compareTwoOrders);

    return orders_.front();
}

// Return the number of orders in the OrdersList
int OrdersList::size() const {
    return orders_.size();
}

// Add an order to the OrderList.
void OrdersList::add(Order *order) {
    orders_.push_back(order);
    contentToLog = toString(order->getType());
    notify();
}

// Move an order within the OrderList from `area` position to `targetRange` position.
void OrdersList::move(int area, int targetRange) {
    bool currentAreaInRange = area >= 0 && area < orders_.size();
    bool destinationInRange = targetRange >= 0 && targetRange < orders_.size();

    if (currentAreaInRange && destinationInRange) {
        auto orderPosition = next(orders_.begin(), area);
        auto targetPosition = next(orders_.begin(), targetRange);

        // Move the order forward if it is ahead of its destination.
        if (targetPosition > orderPosition) {
            while (orderPosition != targetPosition) {
                std::swap(*orderPosition, *next(orderPosition, 1));
                orderPosition++;
            }
        }

        // Move the order backward if it is ahead of its destination.
        if (targetPosition < orderPosition) {
            while (orderPosition != targetPosition) {
                std::swap(*orderPosition, *prev(orderPosition, 1));
                orderPosition--;
            }
        }
    }
}

/**
 * This is to delete an order at specified index from OrderList
 * @param target
 */
void OrdersList::remove(int target) {
    auto orderToRemoveIterator = orders_.begin() + target;
    delete *orderToRemoveIterator;
    orders_.erase(orderToRemoveIterator);
}


/**
 * Implementing Deploy Order class
 */

// Constructors
DeployOrder::DeployOrder() : Order(nullptr, 1), numberOfArmies_(0), destination_(nullptr) {}

DeployOrder::DeployOrder(Player *issuer, int numberOfArmies, Territory *destination) : Order(issuer, 1),
                                                                                       numberOfArmies_(numberOfArmies),
                                                                                       destination_(destination) {}

DeployOrder::DeployOrder(const DeployOrder &order) : Order(order), numberOfArmies_(order.numberOfArmies_),
                                                     destination_(order.destination_) {}

// Operator overloading
const DeployOrder &DeployOrder::operator=(const DeployOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        numberOfArmies_ = order.numberOfArmies_;
        destination_ = order.destination_;
    }
    return *this;
}

std::ostream &DeployOrder::print_(std::ostream &output) const {
    output << "[DeployOrder]";

    if (destination_ != nullptr) {
        output << " " << numberOfArmies_ << " armies to " << destination_->getName();
    }

    return output;
}

/**
 * This is to clone Deploy Order
 * @return pointer
 */

Order *DeployOrder::clone() const {
    return new DeployOrder(*this);
}

// Add a number of armies to deploy to the order
void DeployOrder::addArmies(int additional) {
    numberOfArmies_ += additional;
}

// Checks that the DeployOrder is valid.
//If the target territory does not belong to the player that issued the order, the order is invalid.
bool DeployOrder::validate() const {
    if (issuer_ == nullptr || destination_ == nullptr) {
        return false;
    }

    std::vector<Territory *> currentPlayerTerritories = issuer_->getTerritories();
    return find(currentPlayerTerritories.begin(), currentPlayerTerritories.end(), destination_) !=
           currentPlayerTerritories.end();
}

// Executes the DeployOrder.
//If the target territory belongs to the player that issued the deploy order, the selected number of armies is
//added to the number of armies on that territory.
void DeployOrder::execute_() {
    destination_->addArmies(numberOfArmies_);
    destination_->setPendingIncomingArmies(0);
    std::cout << "Deployed " << numberOfArmies_ << " armies to " << destination_->getName() << "." << std::endl;
}

// Reset the pre-orders-execution game state to the state it was in before the order was placed.
// This order's contribution to the number of pending arriving armies on the target territory is reset.
void DeployOrder::undo_() {
    int newPendingIncomingArmies = destination_->getPendingIncomingArmies() - numberOfArmies_;
    destination_->setPendingIncomingArmies(newPendingIncomingArmies);
}

// Get the type of the Order sub-class
OrderType DeployOrder::getType() const {
    return DEPLOY;
}


/**
 * Implementing Advance Order class
 */

// Constructors
AdvanceOrder::AdvanceOrder() : Order(), numberOfArmies_(0), source_(nullptr), destination_(nullptr) {}

AdvanceOrder::AdvanceOrder(Player *issuer, int numberOfArmies, Territory *source, Territory *destination)
        : Order(issuer, 4), numberOfArmies_(numberOfArmies), source_(source), destination_(destination) {}

AdvanceOrder::AdvanceOrder(const AdvanceOrder &order)
        : Order(order), numberOfArmies_(order.numberOfArmies_), source_(order.source_),
          destination_(order.destination_) {}

// Operator overloading
const AdvanceOrder &AdvanceOrder::operator=(const AdvanceOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        numberOfArmies_ = order.numberOfArmies_;
        source_ = order.source_;
        destination_ = order.destination_;
    }
    return *this;
}

//An advance order tells a certain number of army units to move from a source territory to a target adjacent territory
std::ostream &AdvanceOrder::print_(std::ostream &output) const {
    output << "[AdvanceOrder]";

    if (source_ != nullptr && destination_ != nullptr) {
        output << " " << numberOfArmies_ << " armies from " << source_->getName() << " to " << destination_->getName();
    }

    return output;
}

/**
 * This is to clone Advance Order
 * @return pointer
 */

Order *AdvanceOrder::clone() const {
    return new AdvanceOrder(*this);
}

// Checks if Advance Order function is valid
// If the source territory does not belong to the player that issued the order, the order is invalid.
// If the target territory is not adjacent to the source territory, the order is invalid.
bool AdvanceOrder::validate() const {
    if (issuer_ == nullptr || source_ == nullptr || destination_ == nullptr) {
        return false;
    }

    std::vector<Territory *> currentTerritoriesOfPlayer = issuer_->getTerritories();
    bool validSourceTerritory = find(currentTerritoriesOfPlayer.begin(), currentTerritoriesOfPlayer.end(), source_) !=
                                currentTerritoriesOfPlayer.end();
    bool hasAnyArmiesToAdvance = source_->getNumberOfArmies() > 0;

    return validSourceTerritory && hasAnyArmiesToAdvance && checkIfPossibleToAttack(issuer_, destination_);
}

// Executes the Advance Order.
void AdvanceOrder::execute_() {
    Player *defender = destination_->getOwner();
    bool offensive = issuer_ != defender;

    // Recalculate what number of armies may want to truely be moved if the kingdom of the territory has modified because of an attack
    int movableArmiesFromSource = std::min(source_->getNumberOfArmies(), numberOfArmies_);
    std::cout << "=======An Advanced Order is executed ========";
    if (offensive) {
        // Simulate battle
        source_->removeArmies(movableArmiesFromSource);

        //Each attacking army unit involved has 60% chances of killing one defending army.
        int defendersKilled = round(movableArmiesFromSource * 0.6);
        //At the same time, each defending army unit has 70% chances of killing one attacking army unit.
        int attackersKilled = round(destination_->getNumberOfArmies() * 0.7);

        int survivingAttackers = std::max(movableArmiesFromSource - attackersKilled, 0);
        int survivingDefenders = std::max(destination_->getNumberOfArmies() - defendersKilled, 0);
        destination_->removeArmies(defendersKilled);

        // Failed attack
        if (survivingDefenders > 0 || survivingAttackers <= 0) {
            source_->addArmies(survivingAttackers);
            std::cout << "Failed attack on " << destination_->getName() << " with " << survivingDefenders
                      << " enemy armies left standing.";

            if (survivingAttackers > 0) {
                std::cout << " Retreating " << survivingAttackers << " attacking armies back to " << source_->getName()
                          << std::endl;
            } else {
                std::cout << std::endl;
            }
        }
            // Successful attack: If all the defender's armies are eliminated, the attacker captures the territory
        else {
            std::cout << "=======Successful Attack: (2) Ownership of a territory is transferred to the attacking player if a territory is conquered. ========";
            defender->transferTerritory(destination_, issuer_);
            destination_->addArmies(survivingAttackers);
            std::cout << "Attack is successful on the " << destination_->getName() << ". " << survivingAttackers
                      << " armies now attacked and owns this territory." << std::endl;
        }
    } else {
        source_->removeArmies(movableArmiesFromSource);
        destination_->addArmies(movableArmiesFromSource);
        std::cout << "Advanced " << movableArmiesFromSource << " armies from " << source_->getName() << " to "
                  << destination_->getName() << "." << std::endl;
    }

    source_->setPendingOutgoingArmies(0);
}

// Reset the pre-orders-execution game state to the state it was in before the order was placed.
// This order's contribution to the number of pending outgoing armies from the originating territory is reset.
void AdvanceOrder::undo_() {
    int newPendingOutgoingArmies = source_->getPendingOutgoingArmies() - numberOfArmies_;
    source_->setPendingOutgoingArmies(newPendingOutgoingArmies);
}

// Get the type of the Order sub-class
OrderType AdvanceOrder::getType() const {
    return ADVANCE;
}


/**
 * Implementing Bomb Order class
 */

// Constructors
BombOrder::BombOrder() : Order(), target_(nullptr) {}

BombOrder::BombOrder(Player *issuer, Territory *target) : Order(issuer, 4), target_(target) {}

BombOrder::BombOrder(const BombOrder &order) : Order(order), target_(order.target_) {}

// Operator overloading
const BombOrder &BombOrder::operator=(const BombOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        target_ = order.target_;
    }
    return *this;
}

std::ostream &BombOrder::print_(std::ostream &output) const {
    output << "[BombOrder]";

    if (target_ != nullptr) {
        output << " Target: " << target_->getName();
    }

    return output;
}

/**
 * This is to clone the Bomb Order
 * @return pointer
 */
Order *BombOrder::clone() const {
    return new BombOrder(*this);
}

// Checks that the BombOrder is valid.
//If the target belongs to the player that issued the order, the order is invalid.
//If the target territory is not adjacent to one of the territory owned by the player issuing the order, then the order is invalid.
bool BombOrder::validate() const {
    if (issuer_ == nullptr || target_ == nullptr) {
        return false;
    }

    std::vector<Territory *> currentPlayerTerritories = issuer_->getTerritories();
    bool validTargetTerritory = find(currentPlayerTerritories.begin(), currentPlayerTerritories.end(), target_) ==
                                currentPlayerTerritories.end();
    return validTargetTerritory && checkIfPossibleToAttack(issuer_, target_);
}

// Executes the BombOrder.
//If the target belongs to an enemy player, half of the armies are removed from this territory.
void BombOrder::execute_() {
    int armiesOnTarget = target_->getNumberOfArmies();
    target_->removeArmies(armiesOnTarget / 2);
    std::cout << "Bombed " << armiesOnTarget / 2 << " enemy armies on " << target_->getName() << ". ";
    std::cout << target_->getNumberOfArmies() << " remaining." << std::endl;
}

// Get the type of the Order sub-class
OrderType BombOrder::getType() const {
    return BOMB;
}


/**
 * Implementing Blockade Order class
 */

// Constructors
BlockadeOrder::BlockadeOrder() : Order(nullptr, 3), territory_(nullptr) {}

BlockadeOrder::BlockadeOrder(Player *issuer, Territory *territory) : Order(issuer, 3), territory_(territory) {}

BlockadeOrder::BlockadeOrder(const BlockadeOrder &order) : Order(order), territory_(order.territory_) {}

// Operator overloading
const BlockadeOrder &BlockadeOrder::operator=(const BlockadeOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        territory_ = order.territory_;
    }
    return *this;
}

std::ostream &BlockadeOrder::print_(std::ostream &output) const {
    output << "[BlockadeOrder]";

    if (territory_ != nullptr) {
        output << " Territory: " << territory_->getName() << " (" << territory_->getNumberOfArmies() << " present)";
    }

    return output;
}

/**
 * This is to clone Blockade Order
 * @return pointer
 */
Order *BlockadeOrder::clone() const {
    return new BlockadeOrder(*this);
}

// Checks that the BlockadeOrder is valid.
//If the target territory belongs to an enemy player, the order is declared invalid.
bool BlockadeOrder::validate() const {
    if (issuer_ == nullptr || territory_ == nullptr) {
        return false;
    }

    std::vector<Territory *> currentPlayerTerritories = issuer_->getTerritories();
    return find(currentPlayerTerritories.begin(), currentPlayerTerritories.end(), territory_) !=
           currentPlayerTerritories.end();
}

// Executes the BlockadeOrder.
void BlockadeOrder::execute_() {
    //the number of armies on the territory is doubled and the ownership of the territory is transferred to the Neutral player
    territory_->addArmies(territory_->getNumberOfArmies());
    GameEngine::assignToNeutralPlayer(territory_);
    std::cout << "Blockade called on " << territory_->getName() << ". ";
    std::cout << territory_->getNumberOfArmies() << " neutral armies now occupy this territory." << std::endl;
}

// Get the type of the Order sub-class
OrderType BlockadeOrder::getType() const {
    return BLOCKADE;
}


/**
 * Implementing Airlift Order class
 */

// Constructors
AirliftOrder::AirliftOrder() : Order(nullptr, 2), numberOfArmies_(0), source_(nullptr), destination_(nullptr) {}

AirliftOrder::AirliftOrder(Player *issuer, int numberOfArmies, Territory *source, Territory *destination)
        : Order(issuer, 2), numberOfArmies_(numberOfArmies), source_(source), destination_(destination) {}

AirliftOrder::AirliftOrder(const AirliftOrder &order) : Order(order), numberOfArmies_(order.numberOfArmies_),
                                                        source_(order.source_), destination_(order.destination_) {}

// Operator overloading
const AirliftOrder &AirliftOrder::operator=(const AirliftOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        numberOfArmies_ = order.numberOfArmies_;
        source_ = order.source_;
        destination_ = order.destination_;
    }
    return *this;
}

std::ostream &AirliftOrder::print_(std::ostream &output) const {
    output << "[AirliftOrder]";

    if (source_ != nullptr && destination_ != nullptr) {
        output << " " << numberOfArmies_ << " armies from " << source_->getName() << " to " << destination_->getName();
    }

    return output;
}


/**
 * This is to clone Airlift Order
 * @return pointer
 */

Order *AirliftOrder::clone() const {
    return new AirliftOrder(*this);
}

// Checks that the AirliftOrder is valid.
// If the source or target does not belong to the player that issued the order, the order is invalid.
bool AirliftOrder::validate() const {
    if (issuer_ == nullptr || source_ == nullptr || destination_ == nullptr || source_ == destination_) {
        return false;
    }

    std::vector<Territory *> currentPlayerTerritories = issuer_->getTerritories();

    bool validSourceTerritory = find(currentPlayerTerritories.begin(), currentPlayerTerritories.end(), source_) !=
                                currentPlayerTerritories.end();
    bool validDestinationTerritory =
            find(currentPlayerTerritories.begin(), currentPlayerTerritories.end(), destination_) !=
            currentPlayerTerritories.end();
    bool hasAnyArmiesToAirlift = source_->getNumberOfMovableArmies() > 0;

    return validSourceTerritory && validDestinationTerritory && hasAnyArmiesToAirlift;
}

// Executes the AirliftOrder.
void AirliftOrder::execute_() {
    // Recalculate how many armies could actually be moved in case the state of the territory has changed due to an attack
    int movableArmiesFromSource = std::min(source_->getNumberOfArmies(), numberOfArmies_);

    destination_->addArmies(movableArmiesFromSource);
    source_->removeArmies(movableArmiesFromSource);
    source_->setPendingOutgoingArmies(0);

    //Selected number of armies is moved from the source to the target territory.
    std::cout << "Airlifted " << movableArmiesFromSource << " armies from " << source_->getName() << " to "
              << destination_->getName() << "." << std::endl;
}

// Reset the pre-orders-execution game state to the state it was in before the order was placed.
// This order's contribution to the number of pending outgoing armies from the originating territory is reset.
void AirliftOrder::undo_() {
    int newPendingOutgoingArmies = source_->getPendingOutgoingArmies() - numberOfArmies_;
    source_->setPendingOutgoingArmies(newPendingOutgoingArmies);
}

// Get the type of the Order sub-class
OrderType AirliftOrder::getType() const {
    return AIRLIFT;
}


/**
 * Implementing Negotiate Order class
 */

// Constructors
NegotiateOrder::NegotiateOrder() : Order(), target_(nullptr) {}

NegotiateOrder::NegotiateOrder(Player *issuer, Player *target) : Order(issuer, 4), target_(target) {}

NegotiateOrder::NegotiateOrder(const NegotiateOrder &order) : Order(order), target_(order.target_) {}

// Operator overloading
const NegotiateOrder &NegotiateOrder::operator=(const NegotiateOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        target_ = order.target_;
    }
    return *this;
}

std::ostream &NegotiateOrder::print_(std::ostream &output) const {
    output << "[NegotiateOrder]";

    if (issuer_ != nullptr && target_ != nullptr) {
        output << " Initiator: " << issuer_->getName() << ", Target: " << target_->getName();
    }

    return output;
}

/**
 * This is to clone Negotiate Order
 * @return pointer
 */

Order *NegotiateOrder::clone() const {
    return new NegotiateOrder(*this);
}

// Checks that the NegotiateOrder is valid.
//If the target is the player issuing the order, then the order is invalid.
bool NegotiateOrder::validate() const {
    if (issuer_ == nullptr || target_ == nullptr) {
        return false;
    }

    return issuer_ != target_;
}

// Executes the NegotiateOrder.
//The effect is that any attack that may be declared between territories
//of the player issuing the negotiate order and the target player will result in an invalid order.
void NegotiateOrder::execute_() {
    issuer_->addDiplomaticRelation(target_);
    target_->addDiplomaticRelation(issuer_);
    std::cout << "Negotiated diplomacy between " << issuer_->getName() << " and " << target_->getName() << "."
              << std::endl;
}

// Get the type of the Order sub-class
OrderType NegotiateOrder::getType() const {
    return NEGOTIATE;
}

// for LoggingObserver
string Order::stringToLog() {
    return "Executing Order";
}

string OrdersList::stringToLog(){
    return "Adding to OrdersList: ";
}


string DeployOrder::stringToLog() {
    return "Executing Deploy Order";
}


string AirliftOrder::stringToLog() {
    return "Executing Airlift Order";
}


string NegotiateOrder::stringToLog() {
    return "Executing Negotiate Order";
}


string AdvanceOrder::stringToLog() {
    return "Executing Advance Order";
}


string BombOrder::stringToLog() {
    return "Executing Bomb Order";
}


string BlockadeOrder::stringToLog() {
    return "Executing Blockade Order";
}

// convert enum OrderType to string
string toString (short enumType) {
    switch( enumType )
    {
        case OrderType::DEPLOY:
            return "Deploy";
        case OrderType::ADVANCE:
            return "Advance";
        case OrderType::BOMB:
            return "Bomb";
        case OrderType::BLOCKADE:
            return "Blockade";
        case OrderType::AIRLIFT:
            return "Airlift";
        case OrderType::NEGOTIATE:
            return "Negotiate";
        default:
            return "Not recognized..";
    }
}