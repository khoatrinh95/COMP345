//
// Created by Thong Tran on 2021-09-25.
//

#include "../orders/Orders.h"
#include "../gameengine/GameEngine.h"

void  orderDriver()
{
    // Setup
    Territory* t1 = new Territory(1,"Columbia",4, nullptr);
    Territory* t2 = new Territory(2,"NewYork",5,nullptr);
    Territory* t3 = new Territory(3,"California",4,nullptr);
    t1->addArmies(10);
    t2->addArmies(5);
    t3->addArmies(10);

    Player* player = new Player("Thong");
    Player* enemy = new Player("Khoa");

    // Add
    player->addTerritory(t1);
    player->addTerritory(t2);
    enemy->addTerritory(t3);

//    GameEngine::setPlayers({ player, enemy });

    OrdersList ordersList;
    ordersList.add(new DeployOrder(player, 3, t1));
    ordersList.add(new AdvanceOrder(player, 1, t1, t3));
    ordersList.add(new BombOrder(player, t3));
    ordersList.add(new BlockadeOrder(player, t1));
    ordersList.add(new AirliftOrder(player, 6, t1, t2));
    ordersList.add(new NegotiateOrder(player, enemy));


    // Show the OrderList
    std::cout << "========== " << "Here is original orders list: " << ordersList << " ==========" << std::endl;
    for (const auto &order : ordersList.getOrders())
    {
        std::cout << *order << std::endl;
        std::cout << std::boolalpha << "Order is valid: " << order->validate() << std::endl;
        order->execute();
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
    delete t1;
    delete t2;
    delete t3;

    // This is the end of Orders driver.


}