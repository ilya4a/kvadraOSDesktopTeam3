#include "ClientA.h"
#include "Accel.h"
#include "TcpConnection.h"

#include <atomic>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <thread>

using namespace std::chrono;
using Clock = steady_clock;

static int64_t nowMs() {
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

ClientA::ClientA() : Client(log_dir) {
    logMod = std::ofstream(log_mod_dir / log_mod_name, std::ios::app);
    if (!logMod) {
        std::cerr << "couldn't open a " << log_mod_dir / log_mod_name << std::endl;
    }
}

void ClientA::run(const std::string &host, uint16_t port) {
    TcpCo   nnection conn(-1);
    if (!conn.connectTo(host, port)) {
        log("[A] connect failed");
        return;
    }

    conn.sendLine(ROLE_A);
    log("[A] connected");

    std::atomic<bool> running { true };

    std::thread sender([&]() {
        auto nextSendTime = Clock::now();
        int i = 0;
        while (running) {
            int64_t ts = nowMs();

            double x = std::sin(i * 0.1);
            i++;
            double y = 0.0;
            double z = 0.0;

            AccelData data(ts, x, y, z);

            std::string data_s = data.to_json().dump();

            if (!conn.sendLine(data_s)) {
                running = false;
                return;
            }

            log(data_s);

            nextSendTime += SEND_INTERVAL;
            std::this_thread::sleep_until(nextSendTime);
        }
    });

    log("[A] send failed");

    while (running) {
        std::string response;
        if (!conn.recvLine(response)) {
            log("[A] recv failed");
            break;
        }
        if (logMod.is_open()) {
            logMod << response << std::endl;
        }
    }

    sender.join();
    log("[A] finished");
}
