
#include "ClientB.h"

#include "TcpConnection.h"
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <thread>

#include "Accel.h"


static int64_t nowMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
    ).count();
}

double calc_distance(double x, double y, double z ) {
    return sqrt(x*x + y*y + z*z);
}

void ClientB::run(const std::string& host, uint16_t port) {

    TcpConnection conn(-1);

    if (!conn.connectTo(host, port)) {
        std::cerr << "[B] connect failed\n";
        return;
    }

    conn.sendLine(ROLE_B);
    std::cout << "[B] connected\n";

    for (int i = 0; i < 20; ++i) {
        std::string response;
        if (!conn.recvLine(response)) {
            std::cerr << "[B] recv failed\n";
            return;
        }

        AccelData data = AccelData::from_json(response);

        double dis = calc_distance(data.x, data.y, data.z);

        AccelResult res(data.timestamp, dis);

        if (!conn.sendLine(res.to_json().dump())) {
            std::cerr << "[B] send failed\n";
            return;
        }

        std::cout << "[B] got: " << response << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}