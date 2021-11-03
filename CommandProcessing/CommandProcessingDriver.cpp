//
// Created by Ricky Lam on 2021-11-03.
//

#include "CommandProcessingDriver.h"

void PhasesLoop(Phases* currentPhase, CommandProcessor* comPro) {
    while (*currentPhase != Phases::ASSIGNREINFORCEMENT) {
        switch (*currentPhase) {
            case Phases::START:
            case Phases::MAPLOADED:
            case Phases::MAPVALIDATED:
            case Phases::PLAYERSADDED:
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
    Phases* nextPhase;
    
    if (filename.compare("") != 0) {
        comPro = new FileCommandProcessorAdapter(filename);
    } else {
        comPro = new CommandProcessor();
    }

    while (*currentPhase == Phases::START) {
        PhasesLoop(currentPhase, comPro);

        delete currentPhase;
        currentPhase = new Phases(Phases::WIN);

        *currentPhase = comPro->getCommand(currentPhase);
    }

}

