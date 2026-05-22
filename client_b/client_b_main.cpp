#include "ClientB.h"
#include <string>

int main(int argc, char **argv) {
    uint16_t port = 5001;
    if (argc > 1) {
        port = static_cast<uint16_t>(std::stoi(argv[1]));
    }

    ClientB server(port);
    server.run("0.0.0.0", port);
}
