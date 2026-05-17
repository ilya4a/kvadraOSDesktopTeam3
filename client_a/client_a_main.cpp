#include "ClientA.h"

#include <iostream>

int main(int argc, char* argv[]) {
    std::string host = "127.0.0.1";
    uint16_t port = 5000;

    if (argc >= 2) host = argv[1];
    if (argc >= 3) port = static_cast<uint16_t>(std::stoi(argv[2]));

    ClientA app;
    app.run(host, port);
    return 0;
}