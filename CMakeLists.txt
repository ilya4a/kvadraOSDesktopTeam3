cmake_minimum_required(VERSION 3.16)
project(kvadraOSDesktopTeam3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

add_library(core INTERFACE)
target_include_directories(core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/core)
target_link_libraries(core INTERFACE nlohmann_json::nlohmann_json)

find_package(Threads REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)

set(PROTO_FILE ${CMAKE_CURRENT_SOURCE_DIR}/proto/accel.proto)
set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

add_custom_command(
        OUTPUT
        ${GENERATED_DIR}/accel.pb.cc
        ${GENERATED_DIR}/accel.pb.h
        ${GENERATED_DIR}/accel.grpc.pb.cc
        ${GENERATED_DIR}/accel.grpc.pb.h
        COMMAND protobuf::protoc
        ARGS
        --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/proto
        --cpp_out=${GENERATED_DIR}
        --grpc_out=${GENERATED_DIR}
        --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
        ${PROTO_FILE}
        DEPENDS ${PROTO_FILE}
)

add_library(grpc_proto STATIC
        ${GENERATED_DIR}/accel.pb.cc
        ${GENERATED_DIR}/accel.grpc.pb.cc
)
target_include_directories(grpc_proto PUBLIC ${GENERATED_DIR})
target_link_libraries(grpc_proto PUBLIC protobuf::libprotobuf gRPC::grpc++)

add_library(transport STATIC transport/TcpConnection.cpp)
target_include_directories(transport PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/transport)
target_link_libraries(transport PUBLIC Threads::Threads core)

add_executable(server
        server/main.cpp
        server/Server.cpp
        server/Server.h
)
target_link_libraries(server PRIVATE grpc_proto core Threads::Threads)

add_executable(client_a
        client_a/client_a_main.cpp
        client_a/ClientA.cpp
        transport/Client.h
)
target_link_libraries(client_a PRIVATE grpc_proto core Threads::Threads)

add_executable(client_b
        client_b/client_b_main.cpp
        client_b/ClientB.cpp
)
target_link_libraries(client_b PRIVATE grpc_proto core Threads::Threads)

target_link_libraries(client_a PRIVATE
        grpc_proto
        transport
        core
        Threads::Threads
)

target_link_libraries(client_b PRIVATE
        grpc_proto
        transport
        core
        Threads::Threads
)

add_custom_target(kvadraOSDesktopTeam3 ALL
        DEPENDS server client_a client_b
)