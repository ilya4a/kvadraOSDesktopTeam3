#include "ClientA.h"
#include "../transport/TcpConnection.h"

#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

#include "Accel.h"

static int64_t nowMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
    ).count();
}

void ClientA::run(const std::string& host, uint16_t port) {

    TcpConnection conn(-1);

    if (!conn.connectTo(host, port)) {
        std::cerr << "[A] connect failed\n";
        return;
    }

    conn.sendLine(ROLE_A);
    std::cout << "[A] connected\n";

    for (int i = 0; i < 20; ++i) {
        int64_t ts = nowMs();

        double x = std::sin(0);
        if (i<10) {
            x = std::sin(i);
        }
        double y = 0;
        double z = 0;

        AccelData res(ts, x, y, z);

        if (!conn.sendLine(res.to_json().dump())) {
            std::cerr << "[A] send failed\n";
            return;
        }

        std::string response;
        if (!conn.recvLine(response)) {
            std::cerr << "[A] recv failed\n";
            return;
        }

        std::cout << "[A] got: " << response << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}