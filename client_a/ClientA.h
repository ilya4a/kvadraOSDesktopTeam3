#ifndef KVADRAOSDESKTOPTEAM3_CLIENTA_H
#define KVADRAOSDESKTOPTEAM3_CLIENTA_H

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>

#include "Client.h"

class ClientA : public Client {
    static constexpr double SEND_FREQ_HZ = 50.0;

    static constexpr auto SEND_INTERVAL = std::chrono::duration_cast<std::chrono::steady_clock::duration>(
        std::chrono::duration<double>(1.0 / SEND_FREQ_HZ)
    );

    static constexpr const char *log_dir = "logs/client_a";

    const std::filesystem::path log_mod_dir = "accel";
    const std::filesystem::path log_mod_name = "module.log";
    std::ofstream logModule;

  public:
    ClientA();
    void run(const std::string &host, uint16_t port) override;
    ~ClientA() = default;
};

#endif // KVADRAOSDESKTOPTEAM3_CLIENTA_H
