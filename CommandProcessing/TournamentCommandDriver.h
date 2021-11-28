//
// Created by PC on 2021-11-22.
//

#ifndef COMP345_N11_TOURNAMENTCOMMANDDRIVER_H
#define COMP345_N11_TOURNAMENTCOMMANDDRIVER_H


#include "CommandProcessing.h"

void TournamentCommandDriver(string basicString);
Phases CheckCommandAndReturnPhaseIncludingTournament(Command* command);
void PhasesLoopTourneyDriver(Phases* currentPhase, CommandProcessor* comPro);


#endif //COMP345_N11_TOURNAMENTCOMMANDDRIVER_H
