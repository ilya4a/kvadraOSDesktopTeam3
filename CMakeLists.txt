cmake_minimum_required(VERSION 4.1)
project(kvadraOSDesktopTeam3)

set(CMAKE_CXX_STANDARD 20)

add_executable(kvadraOSDesktopTeam3 server/main.cpp
        client_a/ClientA.cpp
        client_a/ClientA.h
        transport/TcpConnection.cpp
        transport/TcpConnection.h
        client_b/ClientBApp.cpp
        client_b/ClientBApp.h
        client_a/client_a_main.cpp)
