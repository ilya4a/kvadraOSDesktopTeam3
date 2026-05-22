
#include "Server.h"
#include <string>

int main(int argc, char *argv[]) {
    uint16_t port_listen = 5000;
    std::string b_host  = "127.0.0.1";
    uint16_t b_port = 5001;

    if (argc >= 2) port_listen = static_cast<uint16_t>(std::stoi(argv[1]));
    if (argc >= 3) b_host = argv[2];
    if (argc >= 4) b_port = static_cast<uint16_t>(std::stoi(argv[3]));

    Server server(port_listen, b_host, b_port);
    server.run();
}