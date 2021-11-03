//
// Created by Khoa Trinh on 2021-09-19.
//

#include "TestDriver.h"

int main(int argc, char* argv[]){
    cout << "Hello COMP345" << endl;
//    orderDriver();
//    PlayerDriver();
//    CardDriver();
//    mapDriver();
//    GameEngineDriver();

    if (strcmp(argv[1], "-file") == 0) {
        if (argv[2] != NULL) {
            string filename = argv[2];
            CommandProcessingDriver(filename);
        } else {
            cout << "Please specify a file name.";
        }
    } else if (strcmp(argv[1], "-console") == 0) {
        CommandProcessingDriver("");
    } else {
        CommandProcessingDriver("");
    }

}