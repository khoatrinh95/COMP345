//
// Created by Khoa Trinh on 2021-09-24.
//

#ifndef COMP345_N11_STATE_H
#define COMP345_N11_STATE_H
#include "Context.h"

class State {
protected:
    Context *contextPtr;

public:
    virtual ~State();


    void set_context(Context *context);
    void virtual run();
};



#endif //COMP345_N11_STATE_H
