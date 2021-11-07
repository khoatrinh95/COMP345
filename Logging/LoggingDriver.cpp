//
// Created by Khoa Trinh on 2021-11-06.
//

#include "LoggingDriver.h"
#include "../Orders/Orders.h"


void  LoggingDriver()
{
    // Setup
    Territory* columbia = new Territory(1, "Columbia", 0, nullptr);
    Territory* newyork = new Territory(2, "NewYork", 0, nullptr);
    Territory* california = new Territory(3, "California",0, nullptr);

    columbia->addArmies(10);
    newyork->addArmies(5);
    california->addArmies(10);

    Player* player = new Player("Thong");
    Player* enemy = new Player("Khoa");

    // Add
    player->addTerritory(columbia);
    player->addTerritory(newyork);
    enemy->addTerritory(california);


    /*
     * TEST LOGGING OBSERVER FOR ORDER AND ORDERSLIST
     */
    OrdersList ordersList;
    OrdersList* _ordersList = &ordersList;
    LogObserver* logObserver = new LogObserver(_ordersList);
    ordersList.add(new DeployOrder(player, 3, columbia));
    ordersList.add(new AdvanceOrder(player, 1, columbia, california));
    ordersList.add(new BombOrder(player, california));
    ordersList.add(new BlockadeOrder(player, columbia));
    ordersList.add(new AirliftOrder(player, 6, columbia, newyork));
    ordersList.add(new NegotiateOrder(player, enemy));

    GameEngine::addPlayersToList(player);
    GameEngine::addPlayersToList(enemy);
    // Show the OrderList
    std::cout << "========== " << "Here is original orders list: " << ordersList << " ==========" << std::endl;
    for (const auto &order : ordersList.getOrders())
    {
        logObserver = new LogObserver(order);
        std::cout << *order << std::endl;
        std::cout << std::boolalpha << "Order is valid: " << order->validate() << std::endl;
        try{
            order->execute();
        }catch(int e) {
            cout << "An exception occurred. Exception Nr. " << e << '\n';
        }
        std::cout << std::endl;

    }
    delete logObserver;
    logObserver = NULL;

    // After all, delete all territories

    delete columbia;
    columbia = NULL;

    delete newyork;
    newyork = NULL;

    delete california;
    california = NULL;

    GameEngine::clearPlayerList();
}