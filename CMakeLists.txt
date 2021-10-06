cmake_minimum_required(VERSION 3.17)
project(COMP345_N11)

set(CMAKE_CXX_STANDARD 14)

add_executable(COMP345_N11
        TestDriver.cpp TestDriver.h
        Player/Player.cpp Player/Player.h Player/PlayerDriver.h Player/PlayerDriver.cpp
        Orders/Orders.cpp Orders/Orders.h Orders/OrdersDriver.cpp
        #        GameEngine/GameEngine.cpp GameEngine/GameEngine.h
        Cards/CardsDriver.cpp Cards/Cards.h Cards/Cards.cpp
        Map/MapDriver.h Map/MapDriver.cpp Map/Map.cpp Map/Map.h
        )
