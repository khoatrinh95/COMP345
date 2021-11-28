//
// Created by Ricky Lam on 2021-11-03.
//

#ifndef COMP345_N11_COMMANDPROCESSINGDRIVER_H
#define COMP345_N11_COMMANDPROCESSINGDRIVER_H

#include "CommandProcessing.h"

void CommandProcessingDriver(string basicString);
Phases CheckCommandAndReturnPhase(Command* command);
void PhasesLoop(Phases* currentPhase, CommandProcessor* comPro);

#endif //COMP345_N11_COMMANDPROCESSINGDRIVER_H
