//
// Created by sarah on 2021-10-06.
//

#include "GameEngineDriver.h"


void GameEngineDriver(){
    GameEngine *gameEngine = new GameEngine();
    LogObserver *logObserver = new LogObserver(gameEngine);
    gameEngine -> startupPhase();
    cout << *gameEngine;

    cout << "***************************************************************************"<<endl;
    cout<< "***\t\t\t TESTING MAIN GAME LOOP \t\t\t***"<<endl;
    cout << "***************************************************************************"<<endl;

    gameEngine->mainGameLoop();

    delete logObserver;
    delete gameEngine;
}