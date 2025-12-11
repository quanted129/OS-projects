cmake_minimum_required(VERSION 3.20)

project(NamedPipeSystem)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

add_executable(Server
        Server.cpp
        common.h
)

add_executable(Client
        Client.cpp
        common.h
)

add_executable(Tests
        tests/tests.cpp
        common.h
)