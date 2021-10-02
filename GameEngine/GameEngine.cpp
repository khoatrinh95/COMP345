//
// Created by Khoa Trinh on 2021-09-29.
//

#include "GameEngine.h"
#include <algorithm>
#include <filesystem>
#include <time.h>
#include <unordered_set>
#include <string>
Phases* phase = new Phases(Phases::START);

// ----------------------------------START UP----------------------------------------------//
void StartUp::startUp() {
    printTitle();
    while (*phase != Phases::ASSIGNREINFORCEMENT) {
        switch (*phase) {
            case Phases::START:
                loadMap();
                break;
            case Phases::MAPLOADED:
                validateMap();
                break;
            case Phases::MAPVALIDATED:
                createPlayers();
                break;
            case Phases::PLAYERSADDED:
                assignTerritories();
                break;
            default:
                cout << "ERROR" << endl;
                break;
        }
    }
}

void StartUp::loadMap(){
    cout << "***  Inside loadMap  ***" << endl;

    cout << "...loading map" << endl;
    // TBI: load map
    cout << "...map loaded" << endl;

    vector<string> options {"Load a different map", "Validate map"};
    HelperFunctions::printOptions(options);

    HelperFunctions::takeInput(Phases::MAPLOADED);
};

void StartUp::validateMap(){
    cout << "***  Inside validateMap  ***" << endl;
    *phase = Phases::MAPVALIDATED;
};

void StartUp::createPlayers(){
    cout << "***  Inside createPlayers  ***" << endl;
    addPlayers();

    vector<string> options {"Add a different number of players", "Assign territories"};
    HelperFunctions::printOptions(options);

    HelperFunctions::takeInput(Phases::PLAYERSADDED);

};

void StartUp::assignTerritories(){
    cout << "***  Inside assignTerritories  ***" << endl;
    *phase = Phases::ASSIGNREINFORCEMENT;
};

void StartUp::printTitle() {
    cout << ("****************************************") << endl;
    cout << ("*                                      *") << endl;
    cout << ("*               WARZONE                *") << endl;
    cout << ("*                                      *") << endl;
    cout << ("****************************************") << endl;
    cout << ("           Welcome to WARZONE          ") << endl;
}

void StartUp::addPlayers(){
    while (true) {
        cout << "Please enter the number of players: " << endl;
        string input;
        cin >> input;
        if (HelperFunctions::isNumber(input)){

            // TBI: HANDLE ADDING PLAYERS HERE

            cout << input << " players have been added." << endl;
            break;
        }
        else {
            cout << "Invalid input" << endl;
        }
    }
}

// ----------------------------------PLAY----------------------------------------------//

void Play::playGame(){
    printPlayPhaseGreeting();
    while (*phase != Phases::WIN) {
        switch (*phase) {
            case Phases::ASSIGNREINFORCEMENT:
                assignArmies();
                break;
            case Phases::ISSUEORDERS:
                issueOrders();
                break;
            case Phases::EXECUTEORDERS:
                executeOrders();
                break;
            case Phases::WIN:
                cleanUp();
                break;
            default:
                cout << "ERROR" << endl;
                break;
        }
    }
};


void Play::assignArmies(){
    cout << "***  Inside assignArmies  ***" << endl;
    *phase = Phases::ISSUEORDERS;
};

void Play::issueOrders(){
    cout << "***  Inside issueOrders  ***" << endl;

    // TBI: HANDLE ISSUE ORDERS HERE

    vector<string> options {"Issue order", "Complete issue orders"};
    HelperFunctions::printOptions(options);

    HelperFunctions::takeInput(Phases::EXECUTEORDERS);

};

void Play::executeOrders(){
    cout << "***  Inside executeOrders  ***" << endl;

    // TBI: HANDLE EXECUTE ORDERS HERE

    vector<string> options {"Execute orders", "Complete execute orders"};
    HelperFunctions::printOptions(options);

    HelperFunctions::takeInput(Phases::WIN);

};

void Play::cleanUp(){
    cout << "***  Inside cleanUp  ***" << endl;

    // Handle memory leak
    delete phase;
    phase = nullptr;
};

void Play::printPlayPhaseGreeting() {
    cout << ("****************************************") << endl;
    cout << ("*           Setup complete             *") << endl;
    cout << ("*                                      *") << endl;
    cout << ("*             LET'S PLAY               *") << endl;
    cout << ("****************************************") << endl << endl;

}

// --------------------------------HELPER FUNCTIONS-------------------------------------------

void HelperFunctions::printOptions(vector<string> options){
    for (int i = 1; i<= options.size(); i++){
        cout << i << " - " << options[i-1] << endl;
    }
    cout << "Please enter the option number: " << endl;

    // Handle memory leak
    options.clear();

    // This will create a new (temporary) vector and swap its contents with options. The temporary vector is then destroyed, freeing the memory along with it.
    vector<string>().swap(options);
}

bool HelperFunctions::isNumber(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void HelperFunctions::takeInput(Phases nextPhase){
    while (true) {
        string input;
        cin >> input;
        if (input == "1"){
            break;
        }
        else if (input == "2"){
            *phase = nextPhase;
            break;
        }
        else {
            cout << "invalid input" << endl;
            break;
        }
    }
}

// Thong's part


namespace
{
// Helper method for debugging/demo. Pauses game execution until the user presses ENTER on the console.
    void pause()
    {
        std::cout << "Press [Enter] to Continue..." << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }

    // Reads the specified directory and returns a vector of filenames for all the `.map` files.
    std::vector<std::string> getMapFileNames(std::string directory)
    {
        std::vector<std::string> fileNames;
        for (const auto &entry : fs::directory_iterator(directory))
        {
            fileNames.push_back(entry.path().filename().string());
        }

        return fileNames;
    }

    // Select and create a game map based on the user's selection.
    Map* selectMap()
    {
        MapLoader* loader = nullptr;
        Map* map = nullptr;
        const std::string RESOURCES_DIRECTORY = "resources";

        std::cout << "Select a map to play on: " << std::endl;
        std::vector<std::string> maps = getMapFileNames(RESOURCES_DIRECTORY);
        int i = 1;
        for (const auto &map : maps)
        {
            std::cout << "[" << i++ << "] " << map << std::endl;
        }

        while (map == nullptr)
        {
            int selection;
            std::cin >> selection;

            if (std::cin.fail() || selection - 1 < 0 || selection - 1 >= maps.size())
            {
                std::cout << "That was not a valid option. Please try again:" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            try
            {
                std::cout << "Loading map..." << std::endl;
                loader = new MapLoader();
                map = loader->loadMap(RESOURCES_DIRECTORY + "/" + maps.at(selection - 1));
            }
            catch (char const *errorMessage)
            {
                std::cout << "The selected map was invalid. Please try another option:" << std::endl;
            }
            catch (std::string const errorMessage)
            {
                try
                {
                    delete loader;
                    loader = new ConquestFileReaderAdapter();
                    map = loader->loadMap(RESOURCES_DIRECTORY + "/" + maps.at(selection - 1));
                }
                catch(char const *errorMessage)
                {
                    std::cout << "The selected map was invalid. Please try another option:" << std::endl;
                }
                catch (std::string const errorMessage)
                {
                    std::cout << "The selected map was invalid. Please try another option:" << std::endl;
                }

            }

            delete loader;
        }

        return map;
    }

    // Create players based on user's input.
    std::vector<Player*> setupPlayers()
    {
        std::cout << "Enter the number of players for this game: ";

        int numberOfPlayers = 0;

        while (numberOfPlayers == 0)
        {
            std::cin >> numberOfPlayers;

            if (std::cin.fail() || numberOfPlayers < 2 || numberOfPlayers > 5)
            {
                std::cout << "Please enter a valid number of players (2-5): ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                numberOfPlayers = 0;
                continue;
            }

            break;
        }

        std::vector<Player*> players;
        for (int i = 1; i <= numberOfPlayers; i++)
        {
            std::cout << "Enter a name for Player " << i << ": ";
            std::string name;
            std::cin >> name;

            players.push_back(new Player(name, new HumanPlayerStrategy()));
        }

        return players;
    }

    // Turn on/off specified observers
    void setupObservers(GameEngine* gameEngine)
    {
        bool gameStatsObserverOn = true;
        bool phaseObserverOn = true;
        int selection = 0;

        while (selection != 3)
        {
            std::cout << "Configure game observers (enter 1 or 2 to toggle):" << std::endl;
            std::cout << "[1] Game Statistics Observer: " << (gameStatsObserverOn ? "ON" : "OFF") << std::endl;
            std::cout << "[2] Phase Observer: " << (phaseObserverOn ? "ON" : "OFF") << std::endl;
            std::cout << "[3] Confirm" << std::endl;

            while (true)
            {
                std::cin >> selection;
                if (std::cin.fail() || selection < 1 || selection > 3)
                {
                    std::cout << "Please select a valid option: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }

                break;
            }

            gameStatsObserverOn = selection == 1 ? !gameStatsObserverOn : gameStatsObserverOn;
            phaseObserverOn = selection == 2 ? !phaseObserverOn : phaseObserverOn;
        }

        if (gameStatsObserverOn)
        {
            gameEngine->attach(new GameStatisticsObserver(gameEngine));
        }

        if (phaseObserverOn)
        {
            gameEngine->attach(new PhaseObserver(gameEngine));
        }
    }
}




/*
===================================
 Implementation for GameEngine class
===================================
 */

// Constructors
GameEngine::GameEngine() : currentPhase_(NONE), activePlayer_(nullptr) {}

GameEngine::GameEngine(const GameEngine &gameEngine) : currentPhase_(gameEngine.currentPhase_), activePlayer_(gameEngine.activePlayer_) {}

// Operator overloading
const GameEngine &GameEngine::operator=(const GameEngine &gameEngine)
{
    if (this != &gameEngine)
    {
        currentPhase_ = gameEngine.currentPhase_;
        activePlayer_ = gameEngine.activePlayer_;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &output, const GameEngine &gameEngine)
{
    return output;
}

// Assign a territory to the Neutral Player. If no such player exists, create one.
void GameEngine::assignToNeutralPlayer(Territory* territory)
{
    Player* owner = getOwnerOf(territory);
    owner->removeOwnedTerritory(territory);

    auto isNeutralPlayer = [](const auto &player) { return player->isNeutral(); };
    auto iterator = find_if(players_.begin(), players_.end(), isNeutralPlayer);
    if (iterator == players_.end())
    {
        Player* neutralPlayer = new Player();
        neutralPlayer->addOwnedTerritory(territory);
        players_.push_back(neutralPlayer);
    }
    else
    {
        Player* neutralPlayer = *iterator;
        neutralPlayer->addOwnedTerritory(territory);
    }
}

// Find the player who owns the specified territory. Return nullptr if the territory is unowned.
Player* GameEngine::getOwnerOf(Territory* territory)
{
    for (const auto &player : players_)
    {
        std::vector<Territory*> territories = player->getPossessedTerritories();
        if (find(territories.begin(), territories.end(), territory) != territories.end())
        {
            return player;
        }
    }

    return nullptr;
}






