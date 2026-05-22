## Test assignment for the kvadraOS Desktop team 3.
Simple implementation of Level 2 has been completed.

## What is done
- Implemented a distributed accelerometer data pipeline with 3 processes: `server`, `client_a`, and `client_b`.
- Added a gRPC/Protobuf transport layer.
- The communication format is defined in proto/accel.proto: it declares a bidirectional streaming gRPC service and the AccelPacket/AccelModule message schemas used to exchange accelerometer data and computed results between nodes.

## Build requirements
- gRPC and Protobuf installed and visible to CMake
- `nlohmann/json` is downloaded automatically via `FetchContent`

## Build instructions
1. Put the protobuf file here:
    - `proto/accel.proto`

2. Configure and build:
   ```bash
   cmake -S . -B build
   cmake --build build -j

## Run 
```bash
./build/server
./build/client_b
./build/client_a
```
