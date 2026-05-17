#include "../transport/TcpConnection.h"

#include <iostream>
#include <optional>
#include <string>

static std::string readRole(TcpConnection& c) {
    std::string line;
    if (!c.recvLine(line)) return "";
    if (line == "ROLE A") return "A";
    if (line == "ROLE B") return "B";
    return "";
}

int main(int argc, char* argv[]) {
    uint16_t port = 5000;
    if (argc >= 2) port = static_cast<uint16_t>(std::stoi(argv[1]));

    TcpConnection listener(0);
    if (!listener.bindAndListen(port)) {
        std::cerr << "[S] listen failed\n";
        return 1;
    }

    std::cout << "[S] listening on port " << port << "\n";

    std::optional<TcpConnection> connA;
    std::optional<TcpConnection> connB;

    while (!connA || !connB) {
        auto client = listener.acceptClient();
        if (!client) continue;

        std::string role = readRole(*client);
        if (role == "A" && !connA) {
            connA = std::move(*client);
            std::cout << "[S] client A connected\n";
        } else if (role == "B" && !connB) {
            connB = std::move(*client);
            std::cout << "[S] client B connected\n";
        } else {
            std::cerr << "[S] unknown role or duplicate\n";
        }
    }

    while (true) {
        std::string packet;
        if (!connA->recvLine(packet)) {
            std::cerr << "[S] A disconnected\n";
            break;
        }

        std::cout << "[S] from A: " << packet << "\n";

        if (!connB->sendLine(packet)) {
            std::cerr << "[S] send to B failed\n";
            break;
        }

        std::string result;
        if (!connB->recvLine(result)) {
            std::cerr << "[S] B disconnected\n";
            break;
        }

        std::cout << "[S] from B: " << result << "\n";

        if (!connA->sendLine(result)) {
            std::cerr << "[S] send to A failed\n";
            break;
        }
    }

    return 0;
}