//
// Created by sarah on 2021-10-06.
//

#include "GameEngineDriver.h"


void GameEngineDriver(){
    cout << "\n****************************************************************************" << endl;
    cout << "\t\tTesting game initialization" << endl;
    cout << "****************************************************************************" << endl;
    GameEngine *gameEngine = new GameEngine();
    LogObserver *logObserver = new LogObserver(gameEngine);
    gameEngine -> startupPhase();
    cout << *gameEngine;

    cout << "\n****************************************************************************" << endl;
    cout << "\t\tEnd of testing game initialization" << endl;
    cout << "****************************************************************************" << endl;

    cout << "***************************************************************************"<<endl;
    cout<< "***\t\t\t TESTING MAIN GAME LOOP \t\t\t***"<<endl;
    cout << "***************************************************************************"<<endl;

    gameEngine->mainGameLoop();

    delete logObserver;
    delete gameEngine;
}