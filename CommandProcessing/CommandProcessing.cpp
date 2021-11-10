//
// Created by Ricky Lam on 2021-10-27.
//

#include "CommandProcessing.h"
#include <sstream>
#include <string>

/*
 * Command portion
 */

Command::Command() {
    command = "none";
    effect = "none";
    instruction = "";
    argument = "";
}

Command::Command(string command, string effect) {
    this->command = command;
    this->effect = effect;
}

Command::Command(string command, string effect, string instruction) {
    this->command = command;
    this->effect = effect;
    this->instruction = instruction;
}

Command::Command(string command, string effect, string instruction, string argument) {
    this->command = command;
    this->effect = effect;
    this->instruction = instruction;
    this->argument = argument;
}

/**
 * copy constructor, copies all details
 * @param c command object to be copied
 */
Command::Command(const Command& c) {
    this->command = c.command;
    this->effect = c.effect;
    this->instruction = c.instruction;
    this->argument = c.argument;
}

// Definition is commented out because it's only used for debugging
Command::~Command() {
    //cout << "Inside Command destructor of [" << this->getCommand() << "] with effect [" << this->getEffect() << "]" << endl;
}

/**
 * assignment operator that copies attributes
 * @param c object to be copied
 * @return reference to be assigned
 */
Command& Command::operator=(const Command& c) {
    command = c.command;
    effect = c.effect;
    instruction = c.instruction;
    argument = c.argument;
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const Command& c) {
    return stream << "[The command is: " << c.command << ". The effect is: " << c.effect << ".\n";
}

//accessors and mutators
void Command::setCommand(string command) {
    this->command = command;
}

string Command::getCommand() {
    return command;
}

void Command::saveEffect(string effect) {
    this->effect = effect;

    // for Iloggable
    this->contentToLog = effect;
    notify();
}

string Command::getEffect() {
    return effect;
}

/*
 * The following accessor and mutators are part of extra attributes of the command class we added to our design.
 * Please read the descriptions on the private members at CommandProcessing.h
 */

void Command::setInstruction(string instruction) {
    this->instruction = instruction;
}

string Command::getInstruction() {
    return instruction;
}

void Command::setArgument(string argument) {
    this->argument = argument;
}

string Command::getArgument() {
    return argument;
}

/*
 * Command Processor portion
 */

CommandProcessor::CommandProcessor() { }

/**
 * destructor will delete all command objects in the collection
 */
CommandProcessor::~CommandProcessor() {
    for (Command* com : commandList) {
        delete com;
        com = NULL;
    }
    commandList.erase(commandList.begin(), commandList.end());
}

/**
 * assignment operator, copies list of Commands
 */
CommandProcessor& CommandProcessor::operator=(const CommandProcessor& c) {
    vector<Command*> copyOfCommands;
    for (int i = 0; i < c.commandList.size(); i++) {
        copyOfCommands.push_back(new Command(*c.commandList[i]));
    }
    commandList = copyOfCommands;
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const CommandProcessor& com) {
    return stream << "\nThis is a command processor object.\n"; //dummy message, this class does not need to be printed out
}

/**
 * getCommand function for asking for commands from the console
 * @return pointer to the last command object in the collection
 */
Command* CommandProcessor::getCommand() {
    string commander = this->readCommand();
    saveCommand(commander);
    return getLastCommandInList();
}

/**
 * Accessor method to retrieve the list of commands
 * @return vector of pointers of commands
 */
vector<Command*> CommandProcessor::getCommandList() {
    return commandList;
}

/**
 * Accessor method to retrieve the very last added command in the list
 * @return returns pointer to the last command in the list of commands
 */
Command* CommandProcessor::getLastCommandInList() {
    return commandList.back();
}

/**
 * readCommand function to retrieve commands from console inputs
 * @return the string of the command that was read
 */
string CommandProcessor::readCommand() {
    string commander;
    cout << "Enter your command. \n";
    getline(cin, commander); //takes in the entire string until the enter key is pressed in the console
    return commander;
}

/**
 * validate function to verify that the command entered is valid for the game state the game is in
 * @param command a command object we are testing for valid existing command + valid game state to execute in
 * @param phase the current state of the game
 * @return true or false to indicate whether the command entered was valid for the game state
 */
bool CommandProcessor::validate(Command* command, Phases* phase) {
    string commander = command->getCommand();
    stringstream ss(commander);
    string comWord, comArg;
    ss >> comWord; //get first token of input string
    if(comWord == "loadmap" || comWord == "addplayer") {
        ss >> comArg;
    }
    std::transform(comWord.begin(), comWord.end(), comWord.begin(),
                   [](unsigned char c){ return std::tolower(c); }); //convert input to lowercase for easier handling
    if (comWord.compare("loadmap") == 0) {
        if ((*phase != Phases::START && *phase != Phases::MAPLOADED) || comArg.empty()) {
            if(comArg.empty()) {
                cout << "loadmap needs an argument.";
                command->saveEffect("Command does not specify a map file.");
            } else {
                cout << "loadmap is only accepted during the phases START and MAPLOADED.";
                command->saveEffect("Unable to proceed with the command at the current game state.");
            }
            return false;
        } else {
            command->setInstruction(comWord);
            command->setArgument(comArg);
            return true;
        }
    } else if (comWord.compare("validatemap") == 0) {
        if (*phase != Phases::MAPLOADED) {
            cout << "validatemap is only accepted during the phase MAPLOADED.";
            command->saveEffect("Unable to proceed with the command at the current game state.");
            return false;
        } else {
            command->setInstruction(comWord);
            return true;
        }
    } else if (comWord.compare("addplayer") == 0) {
        if ((*phase != Phases::PLAYERSADDED && *phase != Phases::MAPVALIDATED) || comArg.empty()) {
            if(comArg.empty()) {
                cout << "A player needs a name.";
                command->saveEffect("Command does not specify a player's name.");
            } else {
                cout << "addplayer is only accepted during the phases MAPVALIDATED and PLAYERSADDED.";
                command->saveEffect("Unable to proceed with the command at the current game state.");
            }
            return false;
        } else {
            command->setInstruction(comWord);
            command->setArgument(comArg);
            return true;
        }
    } else if (comWord.compare("gamestart") == 0) {
        if (*phase != Phases::PLAYERSADDED) {
            cout << "gamestart is only accepted during the phase PLAYERSADDED.";
            command->saveEffect("Unable to proceed with the command at the current game state.");
            return false;
        } else {
            command->setInstruction(comWord);
            return true;
        }
    } else if (comWord.compare("replay") == 0) {
        if (*phase != Phases::WIN) {
            cout << "replay is only accepted during the phase WIN.";
            command->saveEffect("Unable to proceed with the command at the current game state.");
            return false;
        } else {
            command->setInstruction(comWord);
            return true;
        }
    } else if (comWord.compare("quit") == 0) {
        if (*phase != Phases::WIN) {
            cout << "quit is only accepted during the phase WIN.";
            command->saveEffect("Unable to proceed with the command at the current game state.");
            return false;
        } else {
            command->setInstruction(comWord);
            return true;
        }
    } else {
        cout << "The command entered was not recognized.";
        command->saveEffect("The command entered was not recognized. ");
        return false;
    }
}

/**
 * saves the entered command into a command object that gets added to the command list
 * @param command command that was entered
 */
void CommandProcessor::saveCommand(string command) {
    Command* commandObj = new Command();
    commandObj->setCommand(command);
    commandList.push_back(commandObj);

    // for Iloggable
    contentToLog = command;
    notify();
}

/*
 *  File Line Reader Portion
 */

FileLineReader::FileLineReader() {
    _ifstr = nullptr;
} //zero initialization

/**
 * Initializes the file stream to read from the text file in the parameter
 * @param filename name of the text file to read
 */
FileLineReader::FileLineReader(string filename) {
    this->filename = filename;
    _ifstr = new fstream();
    _ifstr->open(filename, ios::in);
    if (!_ifstr->is_open()) { //Checks to see if file is open, if not, aborts program.
        cout << "File does not exist." << endl;
        exit(0);
    }
}

/**
 * Destructor
 */
FileLineReader::~FileLineReader() {
    _ifstr->close();
    delete _ifstr;
    _ifstr = nullptr;
}

/**
 * Setter method for the file stream
 * @param ifstr
 */
void FileLineReader::setFStr(fstream* ifstr) {
    _ifstr = ifstr;
}

/*
 * The following are helper functions to remove leading and trailing white spaces in the text file's lines
 */

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

/*
 * Back to file line reader
 */

/**
 * reads one singular line from the file
 * @return true if a line was read, false if there was no line to read
 */
bool FileLineReader::readFromLine() {
    string lineBeingRead;
    getline(*_ifstr,lineBeingRead);
    if (lineBeingRead.compare("") != 0) {
        currentLine = trim(lineBeingRead);
        return true;
    } else {
        return false;
    }
}

/**
 * accessor method for the line that was last read
 * @return lines that was last read
 */
string FileLineReader::getCurrentLine() {
    return currentLine;
}

/*
 * FileCommandProcessorAdapter section
 */

/**
 * Non parameterized constructor, sets usingConsole to true and creates a base class version of CommandProcessor
 * so that we can use the console version of getCommand().
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter() {
    flr = nullptr;
    comPro = new CommandProcessor();
    usingConsole = true;
}

/**
 * Parameterized constructor where you can specify file to open. Initializes a filelinereader but not a commandprocessor
 * because we will be reading inputs from the file and not from the console.
 * @param filename file path of the text file to be read
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter(string filename) {
    flr = new FileLineReader(filename);
    comPro = nullptr;
    usingConsole = false;
}

/**
 * Destructor
 */
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete flr;
    flr = nullptr;
    delete comPro;
    comPro = nullptr;
}

/**
 * Public method to start reading commands from text file
 * Calls different versions of getCommand and by extension readCommand by specifying when running the program
 * if it is taking inputs from a file or from the console.
 * @return pointer to the command object that was created from reading the input
 */
Command* FileCommandProcessorAdapter::getCommand() {
    if (usingConsole) {
        return comPro->getCommand();
    } else {
        string commander = this->readCommand();
        saveCommand(commander);
        return getLastCommandInList();
    }
}

/**
 * Adapter pattern: a different version from the console class that is only called when the input comes from a file
 * protected method that calls the file reader class to retrieve commands from the specified file at the constructor
 * @return the string that was read from the line it is currently at
 */
string FileCommandProcessorAdapter::readCommand() {
    if (flr->readFromLine()) {
        string commander = flr->getCurrentLine();
        cout << commander << " has been read from the file." << endl;
        return commander;
    } else {
        cout << "There are no more commands to be read from the file. Aborting program." << endl;
        exit(0);
    }
}

/**
method inherited from ILoggable
 */
string Command::stringToLog() {
    return "Command's effect: ";
}

/**
method inherited from ILoggable
 */
string CommandProcessor::stringToLog() {
    return "Command: ";
}
