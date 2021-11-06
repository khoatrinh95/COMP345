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
}

Command::Command(string command, string effect) {
    this->command = command;
    this->effect = effect;
}

Command::Command(const Command& c) {
    this->command = c.command;
    this->effect = c.effect;
}

Command::~Command() {
}

Command& Command::operator=(const Command& c) {
    command = c.command;
    effect = c.effect;
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

void Command::setEffect(string effect) {
    this->effect = effect;
}

string Command::getEffect() {
    return effect;
}

/*
 * Command Processor portion
 */

CommandProcessor::CommandProcessor() { }

CommandProcessor::~CommandProcessor() { //destructor will delete all command objects in the collection
    for (Command* com : commandList) {
        delete com;
        com = NULL;
    }
    commandList.erase(commandList.begin(), commandList.end());
}

CommandProcessor& CommandProcessor::operator=(const CommandProcessor& c) { //copy constructor, copies list of Commands
    vector<Command*> copyOfCommands;
    for (int i = 0; i < c.commandList.size(); i++) {
        copyOfCommands.push_back(new Command(*c.commandList[i]));
    }
    commandList = copyOfCommands;
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const CommandProcessor& com) {
    return stream << "\nThis is a command processor object.\n";
}

/**
 * getCommand function for asking for commands from the console
 * @return last command object in the collection
 */
Phases CommandProcessor::getCommand(Phases* phase) {
    return this->readCommand(phase);
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
 * @param phase the current game state
 * @return the next game state to transition into
 */
Phases CommandProcessor::readCommand(Phases* phase) {
    string commander;
    cout << "Enter your command. \n";
    getline(cin, commander); //takes in the entire string until the enter key is pressed in the console
    while (!validate(commander, phase)) {
        cout << "The command you entered was invalid. Please try again." << endl;
        cout << "Enter your command. \n";
        getline(cin, commander);
    }
    stringstream ss(commander);
    string comWord;
    ss >> comWord; //get first token of input string
    std::transform(comWord.begin(), comWord.end(), comWord.begin(),
                   [](unsigned char c){ return std::tolower(c); }); //convert input to lowercase

    if (comWord.compare("loadmap") == 0) {
        string secondWord;
        ss >> secondWord; //get second token of input string, which is the map file specified (e.g. canada.map)

        //TODO: check if map file exists

        string header = "Loaded map ";
        string footer = " successfully and transitioned to the maploaded state.";
        string effect = header + secondWord + footer;
        cout << effect << endl;
        saveEffect(getLastCommandInList(), effect);
        return Phases::MAPLOADED;
    } else if (comWord.compare("validatemap") == 0) {

        // TODO : call validate map method

        string effect = "Validated the selected map successfully and transitioned to the mapvalidated state.";
        cout << effect << endl;
        saveEffect(getLastCommandInList(), effect);
        return Phases::MAPVALIDATED;
    } else if (comWord.compare("addplayer") == 0) {
        string secondWord;
        ss >> secondWord; //get second token of input string which is the name of the player to be added

        // TODO : add player to the list of players

        string header = "Added player ";
        string footer = " successfully and transitioned to the playersadded state.";
        string effect = header + secondWord + footer;
        cout << effect << endl;
        saveEffect(getLastCommandInList(), effect);
        return Phases::PLAYERSADDED;
    } else if (comWord.compare("gamestart") == 0) {
        string effect = "Started the game and transitioned to the assignreinforcement state.";
        cout << effect << endl;
        saveEffect(getLastCommandInList(), effect);
        return Phases::ASSIGNREINFORCEMENT;
    } else if (comWord.compare("replay") == 0) {
        string effect = "Replaying the game and transitioning to the start state.";
        cout << effect << endl;
        saveEffect(getLastCommandInList(), effect);
        return Phases::START;
    } else if (comWord.compare("quit") == 0) {
        string effect = "Quitting the game and exiting the program.";
        cout << effect << endl;
        saveEffect(getLastCommandInList(), effect);
        return Phases::WIN; //unsure about this part, there is no exit phase so I made it loop back into WIN
    } else {
        return Phases::START;
    }
}

/**
 * validate function to verify that the command entered is valid for the game state the game is in
 * @param command the command that was input from the console
 * @param phase the current state of the game
 * @return true or false to indicate whether the command entered was valid for the game state
 */
bool CommandProcessor::validate(string command, Phases* phase) {
    stringstream ss(command);
    string comWord;
    ss >> comWord; //get first token of input string
    std::transform(comWord.begin(), comWord.end(), comWord.begin(),
                   [](unsigned char c){ return std::tolower(c); }); //convert input to lowercase for easier handling
    if (comWord.compare("loadmap") == 0) {
        if (*phase != Phases::START && *phase != Phases::MAPLOADED) {
            cout << "loadmap is only accepted during the phases START and MAPLOADED.";
            saveCommand(command);
            saveEffect(commandList.back(), "Unable to proceed with the command at the current game state.");
            return false;
        } else {
            saveCommand(command);
            return true;
        }
    } else if (comWord.compare("validatemap") == 0) {
        if (*phase != Phases::MAPLOADED) {
            cout << "validatemap is only accepted during the phase MAPLOADED.";
            saveCommand(command);
            saveEffect(commandList.back(), "Unable to proceed with the command at the current game state.");
            return false;
        } else {
            saveCommand(command);
            return true;
        }
    } else if (comWord.compare("addplayer") == 0) {
        if (*phase != Phases::PLAYERSADDED && *phase != Phases::MAPVALIDATED) {
            cout << "addplayer is only accepted during the phases MAPVALIDATED and PLAYERSADDED.";
            saveCommand(command);
            saveEffect(commandList.back(), "Unable to proceed with the command at the current game state.");
            return false;
        } else {
            saveCommand(command);
            return true;
        }
    } else if (comWord.compare("gamestart") == 0) {
        if (*phase != Phases::PLAYERSADDED) {
            cout << "gamestart is only accepted during the phase PLAYERSADDED.";
            saveCommand(command);
            saveEffect(commandList.back(), "Unable to proceed with the command at the current game state.");
            return false;
        } else {
            saveCommand(command);
            return true;
        }
    } else if (comWord.compare("replay") == 0) {
        if (*phase != Phases::WIN) {
            cout << "replay is only accepted during the phase WIN.";
            saveCommand(command);
            saveEffect(commandList.back(), "Unable to proceed with the command at the current game state.");
            return false;
        } else {
            saveCommand(command);
            return true;
        }
    } else if (comWord.compare("quit") == 0) {
        if (*phase != Phases::WIN) {
            cout << "quit is only accepted during the phase WIN.";
            saveCommand(command);
            saveEffect(commandList.back(), "Unable to proceed with the command at the current game state.");
            return false;
        } else {
            saveCommand(command);
            return true;
        }
    } else {
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
}

/**
 * saves an effect to the command object
 * @param command command object to add effect to
 * @param effect the effect of the command object
 */
void CommandProcessor::saveEffect(Command* command, string effect) {
    command->setEffect(effect);
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

FileCommandProcessorAdapter::FileCommandProcessorAdapter() {
    flr = nullptr;
    comPro = nullptr;
} //null init

/**
 * Parameterized constructor where you can specify file to open
 * @param filename file path of the text file to be read
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter(string filename) {
    flr = new FileLineReader(filename);
    comPro = new CommandProcessor();
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
 * @param phase specify the phase at which the game is currently in
 * @return the next phase to transition into
 */
Phases FileCommandProcessorAdapter::getCommand(Phases* phase) {
    return this->readCommand(phase);
}

/**
 * private method that calls the file reader class and then validates, adds effects to the commands,
 * and returns the next phase to transition into
 * @param phase specify the phase at which the game is currently in
 * @return the next phase to transition into
 */
Phases FileCommandProcessorAdapter::readCommand(Phases* phase) {
    if (flr->readFromLine()) {
        string commander = flr->getCurrentLine();
        cout << commander << " has been read from the file." << endl;
        if (!validate(commander, phase)) {
            cout << "The command that was read was invalid. Skipping to next line." << endl;
            return *phase;
        }
        stringstream ss(commander);
        string comWord;
        ss >> comWord; //get first token of input string
        std::transform(comWord.begin(), comWord.end(), comWord.begin(),
                       [](unsigned char c){ return std::tolower(c); }); //convert input to lowercase

        if (comWord.compare("loadmap") == 0) {
            string secondWord;
            ss >> secondWord; //get second token of input string, which is the map file specified (e.g. canada.map)

            //TODO: check if map file exists

            string header = "Loaded map ";
            string footer = " successfully and transitioned to the maploaded state.";
            string effect = header + secondWord + footer;
            cout << effect << endl;
            saveEffect(getLastCommandInList(), effect);
            return Phases::MAPLOADED;
        } else if (comWord.compare("validatemap") == 0) {

            // TODO : call validate map method

            string effect = "Validated the selected map successfully and transitioned to the mapvalidated state.";
            cout << effect << endl;
            saveEffect(getLastCommandInList(), effect);
            return Phases::MAPVALIDATED;
        } else if (comWord.compare("addplayer") == 0) {
            string secondWord;
            ss >> secondWord; //get second token of input string which is the name of the player to be added

            // TODO : add player to the list of players

            string header = "Added player ";
            string footer = " successfully and transitioned to the playersadded state.";
            string effect = header + secondWord + footer;
            cout << effect << endl;
            saveEffect(getLastCommandInList(), effect);
            return Phases::PLAYERSADDED;
        } else if (comWord.compare("gamestart") == 0) {
            string effect = "Started the game and transitioned to the assignreinforcement state.";
            cout << effect << endl;
            saveEffect(getLastCommandInList(), effect);
            return Phases::ASSIGNREINFORCEMENT;
        } else if (comWord.compare("replay") == 0) {
            string effect = "Replaying the game and transitioning to the start state.";
            cout << effect << endl;
            saveEffect(getLastCommandInList(), effect);
            return Phases::START;
        } else if (comWord.compare("quit") == 0) {
            string effect = "Quitting the game and exiting the program.";
            cout << effect << endl;
            saveEffect(getLastCommandInList(), effect);
            return Phases::WIN; //unsure about this part, there is no exit phase so I made it loop back into WIN
        }
    } else {
        cout << "There are no more commands to be read from the file. Aborting program." << endl;
        exit(0);
    }
    return Phases::START; //keeping compiler happy
}