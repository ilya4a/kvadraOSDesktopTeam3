#include <Accel.h>

#include <string>
#include "Server.h"

int main(int argc, char* argv[]) {
    uint16_t port = 5000;
    if (argc >= 2) port = static_cast<uint16_t>(std::stoi(argv[1]));

    Server server(port);
    server.run();
}