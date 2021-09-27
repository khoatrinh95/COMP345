//
// Created by Khoa Trinh on 2021-09-24.
//

#ifndef COMP345_N11_CONTEXT_H
#define COMP345_N11_CONTEXT_H
#include "State.h"


class Context {
private:
    State *statePtr;

public:
    Context(State *state);
    ~Context();
    void TransitionTo(State *state);
    void play();
};


#endif //COMP345_N11_CONTEXT_H
