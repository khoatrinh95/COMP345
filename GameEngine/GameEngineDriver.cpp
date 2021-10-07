//
// Created by sarah on 2021-10-06.
//

#include "GameEngineDriver.h"


void GameEngineDriver(){
    GameEngine *gameEngine = new GameEngine();
    gameEngine -> playGame();
    cout << *gameEngine;
    delete gameEngine;
}