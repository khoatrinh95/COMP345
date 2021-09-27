cmake_minimum_required(VERSION 3.17)
project(COMP345_N11)

set(CMAKE_CXX_STANDARD 14)

add_executable(COMP345_N11
        TestDriver.cpp TestDriver.h player/Player.cpp player/Player.h Map.cpp Map.h Cards.cpp Cards.h Orders.cpp Orders.h)
