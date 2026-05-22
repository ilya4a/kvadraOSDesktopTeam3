#include "ClientB.h"

#include <csignal>
#include <string>

void signal_handler(int) {
    std::cout << "\n[B] Interrupt received, exiting.\n";
    std::exit(0);
}

int main(int argc, char **argv) {
    std::signal(SIGINT, signal_handler);

    uint16_t port = 5001;
    if (argc > 1) {
        port = static_cast<uint16_t>(std::stoi(argv[1]));
    }

    ClientB server(port);
    server.run("0.0.0.0", port);
}
