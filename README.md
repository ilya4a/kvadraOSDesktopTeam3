## Test assignment for the kvadraOS Desktop team 3.
A minimal implementation of Level 1 has been completed.

The assignment states a 2‑week deadline. Since I received the test task email on May 8,
I plan to finish the task by May 21.


## Project structure
- **TcpConnection** – RAII TCP socket wrapper with newline‑delimited `sendLine`/`recvLine`.
- **Client** – base class for clients, provides thread‑safe file logging.
- **Server** – accepts A and B, discards consecutive duplicates, relays data.

## Build
```bash
cmake -B build && cmake --build build
```

## Run 
```bash
./build/server          # listens on port 5000
./build/client_b
./build/client_a
```

