//
// Created by Ricky Lam on 2021-10-27.
//

#ifndef COMP345_N11_COMMANDPROCESSING_H
#define COMP345_N11_COMMANDPROCESSING_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../GameEngine/GameEngine.h"
using namespace std;

enum class Phases;

class Command {
public:
    //Constructors and operator overloads
    Command();
    Command(string command, string effect);
    Command(const Command& c);
    ~Command();
    Command& operator =(const Command& c);
    friend std::ostream& operator<<(std::ostream& stream, const Command& c);

    //Accessors and mutators
    void setCommand(string command);
    string getCommand();
    void saveEffect(string effect);
    string getEffect();
private:
    string command;
    string effect;
};

class CommandProcessor {
public:
    //Constructor and destructor
    CommandProcessor();
    virtual ~CommandProcessor();

    //Methods requested from the assignment hand out
    virtual string getCommand(); //public get command method for other classes

    bool validate(string command, Phases* phase);

    Command* getLastCommandInList();

    vector<Command*> getCommandList(); //accessor for list of commands, just in case
    CommandProcessor& operator =(const CommandProcessor& c);
    friend std::ostream& operator<<(std::ostream& stream, const CommandProcessor& com);

protected:
    virtual string readCommand(); //takes input from the console
    void saveCommand(string command); //saves the command received by input into a command object
private:
    vector<Command*> commandList;

};

class FileLineReader {
public:
    //Constructor and destructor
    FileLineReader();
    FileLineReader(string filename);
    ~FileLineReader();

    //methods used for reading the text files
    bool readFromLine();
    string getCurrentLine();

    //mutator for the file input stream
    void setFStr(fstream* ifstr);
private:
    string filename;
    string currentLine;
    fstream* _ifstr; //file input stream from where lines are being read
};

class FileCommandProcessorAdapter : public CommandProcessor {
public:
    //constructor and destructor
    FileCommandProcessorAdapter();
    FileCommandProcessorAdapter(string filename);
    ~FileCommandProcessorAdapter();

    string getCommand(); //public get command method for other classes

    FileCommandProcessorAdapter& operator =(const FileCommandProcessorAdapter& c);
    friend std::ostream& operator<<(std::ostream& stream, const FileCommandProcessorAdapter& com);

protected:
    string readCommand(); //protected function to retrieve commands from a file
private:
    FileLineReader* flr;
    CommandProcessor* comPro;
};




#endif //COMP345_N11_COMMANDPROCESSING_H
