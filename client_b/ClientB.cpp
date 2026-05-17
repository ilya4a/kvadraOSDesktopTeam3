
#include "ClientB.h"

#include "TcpConnection.h"
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>


static int64_t nowMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
    ).count();
}

void ClientB::run(const std::string& host, uint16_t port) {

    TcpConnection conn(0);

    if (!conn.connectTo(host, port)) {
        std::cerr << "[B] connect failed\n";
        return;
    }

    conn.sendLine("ROLE B");
    std::cout << "[B] connected\n";

    for (int i = 0; i < 20; ++i) {
        int64_t ts = nowMs();

        double x = std::sin(i);
        double y = 10;
        double z = std::cos(i);

        std::ostringstream msg;
        msg << ts << " " << x << " " << y << " " << z;

        if (!conn.sendLine(msg.str())) {
            std::cerr << "[B] send failed\n";
            return;
        }

        std::string response;
        if (!conn.recvLine(response)) {
            std::cerr << "[B] recv failed\n";
            return;
        }

        std::cout << "[B] got: " << response << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}