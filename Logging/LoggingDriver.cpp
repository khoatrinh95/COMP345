//
// Created by Khoa Trinh on 2021-11-06.
//

#include "LoggingDriver.h"


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
     * TEST LOGGING OBSERVER
     */
    LogObserver* logObserver;

    DeployOrder* deployOrder = new DeployOrder(player, 3, columbia);
    AdvanceOrder* advanceOrder = new AdvanceOrder(player, 1, columbia, california);
    BombOrder* bombOrder = new BombOrder(player, california);
    OrdersList* ordersList = new OrdersList();

    CommandProcessor* comPro = new CommandProcessor();
    Command* command = new Command();


    vector<Subject*> subjects = {deployOrder, advanceOrder, bombOrder, ordersList, comPro, command};

    for(Subject* s: subjects) {
        logObserver = new LogObserver(s);
    }

    // Test OrderList::add()
    ordersList->add(deployOrder);
    ordersList->add(advanceOrder);
    ordersList->add(bombOrder);

    // Test Order::execute()
    for(Order* order : ordersList->getOrders()){
        order->execute();
    }

    // Test CommandProcessor::saveCommand()
    comPro->getCommand();

    // Test Command::saveEffect()
    command->saveEffect("Testing Logging Effect");

    // Test GameEngine::transition() -> will be tested in GameEngineDriver()

    // clean up - memory leak
    delete ordersList;
    ordersList = NULL;

    delete comPro;
    comPro = NULL;

    delete logObserver;
    logObserver = NULL;

    delete player;
    player = NULL;

    delete enemy;
    enemy = NULL;

    delete columbia;
    columbia = NULL;

    delete newyork;
    newyork = NULL;

    delete california;
    california = NULL;
}