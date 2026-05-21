#ifndef KVADRAOSDESKTOPTEAM3_TCPCONNECTION_H
#define KVADRAOSDESKTOPTEAM3_TCPCONNECTION_H
#include <cstdint>
#include <optional>
#include <string>

class TcpConnection {
  public:
    TcpConnection(const TcpConnection &) = delete;
    TcpConnection &operator=(const TcpConnection &) = delete;
    TcpConnection(TcpConnection &&other) noexcept;

    TcpConnection &operator=(TcpConnection &&other) noexcept;

    TcpConnection(int fd);
    ~TcpConnection();
    bool connectTo(const std::string &host, uint16_t port);
    bool bindAndListen(uint16_t port);

    std::optional<TcpConnection> acceptClient();

    bool sendLine(const std::string &line);
    bool recvLine(std::string &line);

    void close();
    bool valid() const { return fd_ != -1; }

  private:
    int fd_ = -1;
    int listen_fd_ = -1;
};


#endif // KVADRAOSDESKTOPTEAM3_TCPCONNECTION_H
