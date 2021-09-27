//
// Created by Khoa Trinh on 2021-09-24.
//

#include "Context.h"
#include <iostream>
using namespace std;


Context::Context(State *state) : statePtr(nullptr) {
    this->TransitionTo(state);
}

Context::~Context() {
    delete statePtr;
}

 void Context::TransitionTo(State *state) {
    std::cout << "Context: Transition to " << typeid(*state).name() << ".\n";
    if (this->statePtr != nullptr)
        delete this->statePtr;
    this->statePtr = state;
    this->statePtr->set_context(this);
}

void Context::play(){
    statePtr -> run();
}

