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
#include "../Logging/LoggingObserver.h"
using namespace std;

enum class Phases;

class Command : public Subject{
public:
    //Constructors and operator overloads
    Command();
    Command(string command, string effect);

    //please read private members for explanation on the following two
    Command(string command, string effect, string instruction, string argument);
    Command(string command, string effect, string instruction);

    Command(const Command& c);
    ~Command();
    Command& operator =(const Command& c);
    friend std::ostream& operator<<(std::ostream& stream, const Command& c);

    //Accessors and mutators

    void setCommand(string command);
    string getCommand();
    void saveEffect(string effect);
    string getEffect();
    void setInstruction(string instruction);
    string getInstruction();
    void setArgument(string argument);
    string getArgument();

    vector<string> getMapList();
    void setMapList(vector<string> mapList);
    vector<string> getplayerStrategiesList();
    void setplayerStrategiesList(vector<string> playerStrategiesList);
    int getNumOfGames();
    void setNumOfGames(int num);
    int getNumOfTurns();
    void setNumOfTurns(int num);

    // ILoggable
    virtual string stringToLog();
private:
    string command; //entire line of input from console or file by the user
    string effect; //effect of the command, a description
    string instruction; //first word of the command, tells engine what to do
    string argument; //second word of the command if any, an additional parameter such as file or name

    //the following member attributes and functions are for the tournament command
    vector<string> mapList;
    vector<string> playerStrategiesList;
    int numOfGames;
    int numOfTurns;
};

class CommandProcessor : public Subject{
public:
    //Constructor and destructor
    CommandProcessor();
    virtual ~CommandProcessor();

    //Methods requested from the assignment hand out
    virtual Command* getCommand(); //public get command method for other classes

    bool validate(Command* command, Phases* phase);

    Command* getLastCommandInList();

    vector<Command*> getCommandList(); //accessor for list of commands, just in case
    CommandProcessor& operator =(const CommandProcessor& c);
    friend std::ostream& operator<<(std::ostream& stream, const CommandProcessor& com);

    // ILoggable
    virtual string stringToLog();

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

    //adapter pattern: explained in definition
    Command* getCommand(); //public get command method for other classes

    FileCommandProcessorAdapter& operator =(const FileCommandProcessorAdapter& c);
    friend std::ostream& operator<<(std::ostream& stream, const FileCommandProcessorAdapter& com);

protected:
    //adapter pattern: explained in definition
    string readCommand(); //protected function to retrieve commands from a file
private:
    bool usingConsole; //for adapter pattern
    FileLineReader* flr;
    CommandProcessor* comPro;
};




#endif //COMP345_N11_COMMANDPROCESSING_H
