//
// Created by Ricky Lam on 2021-11-03.
//

#include "CommandProcessingDriver.h"

void PhasesLoop(Phases* currentPhase, CommandProcessor* comPro) {
    while (*currentPhase != Phases::ASSIGNREINFORCEMENT) {
        switch (*currentPhase) {
            case Phases::START:
                cout << "Currently in START phase." << endl;
                *currentPhase = comPro->getCommand(currentPhase);
                break;
            case Phases::MAPLOADED:
                cout << "Currently in MAPLOADED phase." << endl;
                *currentPhase = comPro->getCommand(currentPhase);
                break;
            case Phases::MAPVALIDATED:
                cout << "Currently in MAPVALIDATED phase." << endl;
                *currentPhase = comPro->getCommand(currentPhase);
                break;
            case Phases::PLAYERSADDED:
                cout << "Currently in PLAYERSADDED phase." << endl;
                *currentPhase = comPro->getCommand(currentPhase);
                break;
            default:
                cout << "ERROR" << endl;
                break;
        }
    }
}

void CommandProcessingDriver(string filename) {
    CommandProcessor* comPro;
    Phases* currentPhase = new Phases(Phases::START);
    
    if (filename.compare("") != 0) {
        comPro = new FileCommandProcessorAdapter(filename);
    } else {
        comPro = new CommandProcessor();
    }

    while (*currentPhase == Phases::START) {
        PhasesLoop(currentPhase, comPro);

        delete currentPhase;

        cout << "Skipping gameplay loop... Currently in WIN phase." << endl;
        currentPhase = new Phases(Phases::WIN);

        *currentPhase = comPro->getCommand(currentPhase);
    }

    delete currentPhase;
    currentPhase = nullptr;
    delete comPro;
    comPro = nullptr;
}

