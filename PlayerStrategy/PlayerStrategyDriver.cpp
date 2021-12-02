#include "PlayerStrategyDriver.h"


void PlayerStrategiesDriver()
{
    // Setup
    GameEngine gameEngine;
    MapLoader loader;

    Player* player1 = new Player("Player 1");
    Player* player2 = new Player("Player 2");
    Player* player3 = new Player("Player 3");
    Player* player4 = new Player("Player 4");

    player1->setStrategy( Human);
    player2->setStrategy(Aggressive);
    player3->setStrategy(Benevolent);
    player4->setStrategy(Neutral);

    gameEngine.loadMap("../Maps/maps/canada.map");
    gameEngine.addPlayersToList(player1);
    gameEngine.addPlayersToList(player2);
    gameEngine.addPlayersToList(player3);
    gameEngine.addPlayersToList(player4);
    gameEngine.startupPhase();

    // Display the players at the start of the game
    std::cout << std::endl;
    for (const auto &player : gameEngine.getPlayers())
    {
        std::cout << *player << std::endl;
    }

    // ======================================================================================================

    // Round 1
    std::cout << "\n============================ Simulated game: Round 1 ============================" << std::endl;
    std::cout << "\n~~~ Reinforcement phase ~~~\n" << std::endl;
    gameEngine.reinforcementPhase();

    std::cout << "\n~~~ Issue orders phase ~~~\n" << std::endl;
    gameEngine.issueOrdersPhase();

    std::cout << "\n~~~ Execute orders phase ~~~\n" << std::endl;
    gameEngine.executeOrdersPhase();



    // ======================================================================================================

    // Change the strategy for one of the players
    player4->setStrategy(Human);

    // Round 2
    std::cout << "\n============================ Simulated game: Round 2 ============================" << std::endl;
    std::cout << "\n~~~ Reinforcement phase ~~~\n" << std::endl;
    gameEngine.reinforcementPhase();

    std::cout << "\n~~~ Issue orders phase ~~~\n" << std::endl;
    gameEngine.issueOrdersPhase();

    std::cout << "\n~~~ Execute orders phase ~~~\n" << std::endl;
    gameEngine.executeOrdersPhase();


    // todo: do we need to reset? Clean up dynamically allocated memory
}
