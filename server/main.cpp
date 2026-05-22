
#include "Server.h"
#include <string>

#include <csignal>
#include <memory>

std::unique_ptr<grpc::Server> g_grpc_server;

void signal_handler(int signum) {
    std::cout << "\n[S] Received signal " << signum << ", shutting down..." << std::endl;
    if (g_grpc_server) {
        g_grpc_server->Shutdown();
    }
}

int main(int argc, char *argv[]) {
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    uint16_t port_listen = 5000;
    std::string b_host  = "127.0.0.1";
    uint16_t b_port = 5001;

    if (argc >= 2) port_listen = static_cast<uint16_t>(std::stoi(argv[1]));
    if (argc >= 3) b_host = argv[2];
    if (argc >= 4) b_port = static_cast<uint16_t>(std::stoi(argv[3]));

    Server server(port_listen, b_host, b_port);
    server.run();
}