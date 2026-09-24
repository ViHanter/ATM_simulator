cmake_minimum_required(VERSION 3.16)
project(ATM_simulator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ftxui CONFIG REQUIRED)

add_executable(atm_simulator
    src/window/main_screen.cpp
    src/bank.cpp
    src/atm.cpp
    src/account.cpp
    src/card.cpp
)

target_include_directories(atm_simulator PRIVATE src)
target_link_libraries(atm_simulator PRIVATE ftxui::dom ftxui::screen)