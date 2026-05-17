cmake_minimum_required(VERSION 3.16)
project(kvadraOSDesktopTeam3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_library(transport STATIC
        transport/TcpConnection.cpp
)

target_include_directories(transport PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/transport
)

target_link_libraries(transport PUBLIC Threads::Threads)

add_executable(server
        server/main.cpp
)

target_link_libraries(server PRIVATE transport)

add_executable(client_a
        client_a/client_a_main.cpp
        client_a/ClientA.cpp
)

target_include_directories(client_a PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/client_a
        ${CMAKE_CURRENT_SOURCE_DIR}/transport
)

target_link_libraries(client_a PRIVATE transport)

add_executable(client_b
        client_b/client_b_main.cpp
        client_b/ClientB.cpp
)

target_include_directories(client_b PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/client_b
        ${CMAKE_CURRENT_SOURCE_DIR}/transport
)

target_link_libraries(client_b PRIVATE transport)

add_custom_target(kvadraOSDesktopTeam3 ALL
        DEPENDS server client_a client_b
)