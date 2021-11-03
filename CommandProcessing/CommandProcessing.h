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
    void setEffect(string effect);
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
    virtual Phases getCommand(Phases* phase); //public get command method for other classes
    void saveCommand(string command); //saves the command received by input into a command object
    void saveEffect(Command* command, string effect); //saves the effect of a command into its object
    bool validate(string command, Phases* phase);

    Command* getLastCommandInList();

    vector<Command*> getCommandList(); //accessor for list of commands, just in case
    CommandProcessor& operator =(const CommandProcessor& c);
    friend std::ostream& operator<<(std::ostream& stream, const CommandProcessor& com);
private:
    vector<Command*> commandList;
    virtual Phases readCommand(Phases* phase);
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

    virtual Phases getCommand(Phases* phase); //public get command method for other classes

    FileCommandProcessorAdapter& operator =(const FileCommandProcessorAdapter& c);
    friend std::ostream& operator<<(std::ostream& stream, const FileCommandProcessorAdapter& com);
private:
    Phases readCommand(Phases* phase);
    FileLineReader* flr;
    CommandProcessor* comPro;
};




#endif //COMP345_N11_COMMANDPROCESSING_H
