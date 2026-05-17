#ifndef KVADRAOSDESKTOPTEAM3_CLIENTA_H
#define KVADRAOSDESKTOPTEAM3_CLIENTA_H

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <string>

class ClientA {

    static constexpr double SEND_FREQ_HZ = 50.0;

    static constexpr auto SEND_INTERVAL = std::chrono::duration_cast<std::chrono::steady_clock::duration>(
    std::chrono::duration<double>(1.0 / SEND_FREQ_HZ));

    static constexpr int TOTAL_PACKETS = 1000;

    const std::filesystem::path log_file_name = "module.log";
    const std::filesystem::path accel_dir = "accel";

public:
    ClientA();
    void run(const std::string& host, uint16_t port);

};

#endif // KVADRAOSDESKTOPTEAM3_CLIENTA_H
