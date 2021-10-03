cmake_minimum_required(VERSION 3.17)
project(COMP345_N11)

set(CMAKE_CXX_STANDARD 14)

add_executable(COMP345_N11
        TestDriver.cpp TestDriver.h
#        GameEngine/State.cpp GameEngine/State.h
#        GameEngine/Context.cpp GameEngine/Context.h
        Player/Player.cpp Player/Player.h
        Player/PlayerDriver.cpp Player/PlayerDriver.h
        Cards/Cards.cpp Cards/Cards.h
        Map/Map.h Map/Map.cpp Player/PlayerDriver.cpp Player/PlayerDriver.h
        Orders.cpp Orders.h)
