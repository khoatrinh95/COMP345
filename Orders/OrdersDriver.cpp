//
// Created by Thong Tran on 2021-09-25.
//

#include "../orders/Orders.h"
#include "../gameengine/GameEngine.h"

void  orderDriver()
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

    OrdersList ordersList;
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
        std::cout << *order << std::endl;
        std::cout << std::boolalpha << "Order is valid: " << order->validate() << std::endl;
        try{
            order->execute();
        }catch(int e) {
            cout << "An exception occurred. Exception Nr. " << e << '\n';
        }
        std::cout << std::endl;
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
    columbia = NULL;
    delete columbia;
    newyork = NULL;
    delete newyork;
    california = NULL;
    delete california;

   GameEngine::clearPlayerList();
}