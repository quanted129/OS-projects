cmake_minimum_required(VERSION 4.0)
project(SalaryCalculatorProcesses)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(MainProcess
        Main/header.h
        Main/Main.cpp)

add_executable(Creator
        Creator/Creator.cpp
        Main/header.h)

add_executable(Reporter
        Reporter/Reporter.cpp
        Main/header.h)