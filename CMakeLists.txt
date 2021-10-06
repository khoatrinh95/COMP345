cmake_minimum_required(VERSION 3.17)
project(COMP345_N11)

set(CMAKE_CXX_STANDARD 14)

add_executable(COMP345_N11
        TestDriver.cpp TestDriver.h GameEngine/State.cpp GameEngine/State.h GameEngine/Context.cpp GameEngine/Context.h Player/Player.cpp Player/Player.h OrderList/Orders.cpp OrderList/Orders.h OrderList/OrdersDriver.cpp Map/map_loader/MapLoader.cpp Map/map_loader/MapLoader.h Map/map_loader/LoadMapDriver.cpp Observer/Observers.cpp Observer/Observers.h)

