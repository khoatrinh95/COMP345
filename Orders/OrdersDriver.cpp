//
// Created by Thong Tran on 2021-09-25.
//

#include "../orders/Orders.h"
#include "../gameengine/GameEngine.h"

void  orderDriver()
{
    // Setup
    std::cout << "===== " << "3 territories are added: Columbia, newyork and california =====" << std::endl;
    Territory* columbia = new Territory(1, "Columbia", 0, nullptr);
    Territory* newyork = new Territory(2, "NewYork", 0, nullptr);
    Territory* california = new Territory(3, "California",0, nullptr);

    std::cout << "===== " << "Columbia has 10 armies =====" << std::endl;
    columbia->addArmies(10);
    std::cout << "===== " << "Newyork has 5 armies =====" << std::endl;
    newyork->addArmies(5);
    std::cout << "===== " << "California has 10 armies =====" << std::endl;
    california->addArmies(10);
    std::cout << "===== " << "Player 1: Thong is added=====" << std::endl;
    Player* player = new Player("Thong");
    std::cout << "===== " << "Player 2: Khoa is added =====" << std::endl;
    Player* enemy = new Player("Khoa");


    // Add
    std::cout << "===== " << "Assign Player Thong with Columbia and Newyork=====" << std::endl;
    player->addTerritory(columbia);
    player->addTerritory(newyork);
    std::cout << "===== " << "Assign Player Khoa with California =====" << std::endl;
    enemy->addTerritory(california);

    OrdersList ordersList;
    std::cout << "===== " << "Player Thong added a deploy order: 3 armies to Columbia =====" << std::endl;
    ordersList.add(new DeployOrder(player, 3, columbia));
    std::cout << "===== " << "Player Thong added an advanced order: 1 armies from Columbia to California =====" << std::endl;
    ordersList.add(new AdvanceOrder(player, 1, columbia, california));
    std::cout << "===== " << "Player Thong added a bomb order to california =====" << std::endl;
    ordersList.add(new BombOrder(player, california));
    std::cout << "===== " << "Player Thong added a blockade order to Columbia =====" << std::endl;
    ordersList.add(new BlockadeOrder(player, columbia));
    std::cout << "===== " << "Player Thong added a airlift order: 6 armies from Columbia to Newyork =====" << std::endl;
    ordersList.add(new AirliftOrder(player, 6, columbia, newyork));
    std::cout << "===== " << "Player Thong added a negotiate order targeting Player Khoa =====" << std::endl;
    ordersList.add(new NegotiateOrder(player, enemy));


    // Test invalid
  //  ordersList.add(new DeployOrder(player, 3, california));

    std::cout << "===== " << "6 orders are added to the OrderList=====" << std::endl;

//    // adding random cards to player
//    vector<Card*>cards;
//    cards.push_back(new Card("bomb"));
//    cards.push_back(new Card("reinforcement"));
//    Hand * handOfCards = new Hand(cards);
//    player->setPlayerCards(handOfCards);
//    std::cout << "===== " << "Player Thong has some random cards =====" << std::endl;
//
//    // adding random cards to enemy
//    vector<Card*>cardsEnemy;
//    cards.push_back(new Card("bomb"));
//    cards.push_back(new Card("reinforcement"));
//    Hand * handOfCardsEnemy = new Hand(cardsEnemy);
//    enemy->setPlayerCards(handOfCardsEnemy);
//    std::cout << "===== " << "Player Khoa has some random cards =====" << std::endl;



//    GameEngine::addPlayersToList(player);
//    GameEngine::addPlayersToList(enemy);
    // Show the OrderList

    std::cout << "========== " << "Here is original orders list: " << ordersList << " ==========" << std::endl;
    for (const auto &order : ordersList.getOrders())
    {
        std::cout << *order << std::endl;
        std::cout << "===== " << "(1) Verify if order is validated before being executed =====" << std::endl;
        if (order->validate()) {
            std::cout << std::boolalpha << "Order is valid. Execute the order."<< std::endl;
            try{
                order->execute();
            }catch(int e) {
                cout << "An exception occurred. Exception Nr. " << e << '\n';
            }
        } else {
            std::cout << std::boolalpha << "Order is not valid. No execution!" << std::endl;
        }

        std::cout << std::endl;
    }

    std::cout << "========== " << "Finish one turn of a game !!!!!==========" << std::endl;
    std::cout << "========== " << "One card is given to a player if they conquer at least 1 territory in a turn ==========" << std::endl;
    std::cout << "========== " << "Size " << player->getTerritories().size() << "==========" << std::endl;
    std::cout << "========== " << "Size " << enemy->getTerritories().size() << "==========" << std::endl;
    //(3) one card is given to a player if they conquer at least one territory in a turn (not more than one card per turn)
    if (player->getTerritories().size() >=1) {
        std::cout << "========== " << "Player Thong wins 1 more territory in this turn because originally he has only 2 territories. He will be given 1 card ==========" << std::endl;
        vector<Card*>cards;
        // Add random card
        cards.push_back(new Card("bomb"));
        Hand * handOfCards = new Hand(cards);
        player->setPlayerCards(handOfCards);
    } else if (enemy->getTerritories().size() >= 1) {
        std::cout << "========== " << "Player Khoa wins 1 more territory in this turn because originally he has only 1 territory. He will be given 1 card ==========" << std::endl;
        vector<Card*>cards;
        // Add random card
        cards.push_back(new Card("bomb"));
        Hand * handOfCards = new Hand(cards);
        enemy->setPlayerCards(handOfCards);
    }

    // Move the order from index 2 to 4
    ordersList.move(2, 4);

    // Display the OrderList after moving order
    std::cout << "===== " << "Orders list after moving an order: " << ordersList << " =====" << std::endl;
    for (const auto &order : ordersList.getOrders())
    {
        std::cout << *order << std::endl;
    }




    // Delete the order at index 3
    ordersList.remove(3);
    std::cout << "\n===== " << "Orders list after deleting an order: " << ordersList << " =====" << std::endl;
    for (const auto &order : ordersList.getOrders())
    {
        std::cout << *order << std::endl;
    }

    // After all, delete all territories

    delete columbia;
    columbia = NULL;

    delete newyork;
    newyork = NULL;

    delete california;
    california = NULL;

//    GameEngine::clearPlayerList();
}