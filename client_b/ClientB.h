#ifndef KVADRAOSDESKTOPTEAM3_CLIENTBAPP_H
#define KVADRAOSDESKTOPTEAM3_CLIENTBAPP_H

#include "Client.h"

#include <cstdint>
#include <filesystem>
#include <string>

class ClientB : public Client {
    static constexpr const char *log_dir = "logs/client_b";

  public:
    ClientB();
    void run(const std::string &host, uint16_t port) override;
    ~ClientB() = default;
};

#endif // KVADRAOSDESKTOPTEAM3_CLIENTBAPP_H
