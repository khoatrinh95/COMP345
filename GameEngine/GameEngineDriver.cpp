//
// Created by sarah on 2021-10-06.
//

#include "GameEngineDriver.h"


void GameEngineDriver(){
    GameEngine *gameEngine = new GameEngine();
    LogObserver *logObserver = new LogObserver(gameEngine);
    gameEngine -> startupPhase();
    cout << *gameEngine;
    delete logObserver;
    delete gameEngine;
}