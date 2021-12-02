cmake_minimum_required(VERSION 3.17)
project(COMP345_N11)

set(CMAKE_CXX_STANDARD 14)

add_executable(COMP345_N11
        TestDriver.cpp TestDriver.h
        Player/Player.cpp
        Player/Player.h
        Player/PlayerDriver.h
        Player/PlayerDriver.cpp
        Orders/Orders.cpp
        Orders/Orders.h
        Orders/OrdersDriver.cpp
        Orders/OrdersDriver.h
        GameEngine/GameEngine.cpp
        GameEngine/GameEngine.h
        GameEngine/GameEngineDriver.cpp
        GameEngine/GameEngineDriver.h
        Cards/CardsDriver.cpp
        Cards/CardDriver.h
        Cards/Cards.h
        Cards/Cards.cpp
        Map/MapDriver.h
        Map/MapDriver.cpp
        Map/Map.cpp
        Map/Map.h
        CommandProcessing/CommandProcessing.cpp
        CommandProcessing/CommandProcessing.h
        CommandProcessing/CommandProcessingDriver.cpp
        CommandProcessing/CommandProcessingDriver.h
        Logging/LoggingObserver.h
        Logging/LoggingObserver.cpp
        Logging/LoggingDriver.cpp
        Logging/LoggingDriver.h
        PlayerStrategy/PlayerStrategy.cpp
        PlayerStrategy/PlayerStrategyDriver.cpp
        PlayerStrategy/PlayerStrategyDriver.h
        PlayerStrategy/PlayerStrategy.h CommandProcessing/TournamentCommandDriver.cpp CommandProcessing/TournamentCommandDriver.h PlayerStrategy/PlayerStrategyDriver.h)