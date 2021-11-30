//
// Created by sarah on 2021-10-06.
//

#include "GameEngineDriver.h"


void GameEngineDriver(){
    cout << "\n****************************************************************************" << endl;
    cout << "\t\tTesting Part 2" << endl;
    cout << "****************************************************************************" << endl;
    GameEngine *gameEngine = new GameEngine();
    LogObserver *logObserver = new LogObserver(gameEngine);
    gameEngine->gamePlay();

//    cout << *gameEngine;


    delete logObserver;
    delete gameEngine;
}