//
// Created by Khoa Trinh on 2021-09-24.
//

#include "State.h"

#include <iostream>
using namespace std;

void State::run() {};

class PlayState : public State {
public:
    void run() override{
        //assign reinforcement
        cout << "Assigning reinforcement...";

        //issue order
        cout << "Issuing order...";

        //execute order
        cout << "Executing order...";

        //win
        cout << "Winning...";
    }
};

class StartupState : public State {
public:
    void run() override{
        //start
        cout << "Starting...";

        //map loaded
        cout << "Loading maps...";

        //map validated
        cout << "Validating maps...";

        //players added
        cout<< "Adding players...";
        this->contextPtr->TransitionTo(new PlayState);
    }
};

void playGame(){
    Context *context = new Context();
    context -> play();
}

