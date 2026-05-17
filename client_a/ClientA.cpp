#include "ClientA.h"
#include "../transport/TcpConnection.h"
#include "Accel.h"

#include <atomic>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <thread>

using namespace std::chrono;
using Clock = steady_clock;

static int64_t nowMs() {
    return duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
    ).count();
}

ClientA::ClientA() {
    std::filesystem::create_directory(accel_dir);
}

void ClientA::run(const std::string& host, uint16_t port) {
    TcpConnection conn(-1);
    if (!conn.connectTo(host, port)) {
        std::cerr << "[A] connect failed\n";
        return;
    }

    conn.sendLine(ROLE_A);
    std::cout << "[A] connected\n";

    std::atomic<bool> running{true};

    std::thread sender([&]() {
        auto nextSendTime = Clock::now();
        for (int i = 0; i < TOTAL_PACKETS && running; ++i) {
            int64_t ts = nowMs();

            double x = std::sin(i * 0.1);
            double y = 0.0;
            double z = 0.0;

            AccelData data(ts, x, y, z);
            if (!conn.sendLine(data.to_json().dump())) {
                std::cerr << "[A] send failed\n";
                running = false;
                return;
            }

            nextSendTime += SEND_INTERVAL;
            std::this_thread::sleep_until(nextSendTime);
        }
        running = false;
    });

    std::ofstream log(accel_dir / log_file_name, std::ios::app);

    if (!log) {
        std::cerr << "[A] failed to open log file\n";
    }

    while (running) {
        std::string response;
        if (!conn.recvLine(response)) {
            std::cerr << "[A] recv failed\n";
            break;
        }
        std::cout << "[A] got: " << response << "\n";
        if (log.is_open()) {
            log << response << std::endl;
        }
    }

    sender.join();
    std::cout << "[A] finished\n";
}