#include "TcpConnection.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

TcpConnection::TcpConnection(TcpConnection &&other) noexcept : fd_(other.fd_), listen_fd_(other.listen_fd_) {
    other.fd_ = -1;
    other.listen_fd_ = -1;
}

TcpConnection &TcpConnection::operator=(TcpConnection &&other) noexcept {
    if (this != &other) {
        close();
        fd_ = other.fd_;
        listen_fd_ = other.listen_fd_;

        other.fd_ = -1;
        other.listen_fd_ = -1;
    }
    return *this;
}

TcpConnection::TcpConnection(int fd) : fd_(fd) { }

TcpConnection::~TcpConnection() {
    close();
}

bool TcpConnection::connectTo(const std::string &host, uint16_t port) {
    close();

    fd_ = ::socket(AF_INET, SOCK_STREAM, 0);

    if (fd_ < 0) {
        std::perror("socket");
        return false;
    }

    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (::inet_pton(AF_INET, host.c_str(), &addr.sin_addr) != 1) {
        std::cerr << "inet_pton failed for host: " << host << "\n";
        close();
        return false;
    }

    if (::connect(fd_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        std::perror("connect");
        close();
        return false;
    }

    return true;
}

bool TcpConnection::bindAndListen(uint16_t port) {
    close();

    listen_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd_ < 0) {
        std::perror("socket");
        return false;
    }

    int opt = 1;
    setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(listen_fd_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        std::perror("bind");
        close();
        return false;
    }

    if (listen(listen_fd_, 16) < 0) {
        std::perror("listen");
        close();
        return false;
    }

    return true;
}

std::optional<TcpConnection> TcpConnection::acceptClient() {
    if (listen_fd_ < 0) {
        return std::nullopt;
    }

    sockaddr_in client_addr {};
    socklen_t len = sizeof(client_addr);

    int client_fd = ::accept(listen_fd_, reinterpret_cast<sockaddr *>(&client_addr), &len);
    if (client_fd < 0) {
        std::perror("accept");
        return std::nullopt;
    }

    return TcpConnection(client_fd);
}

bool TcpConnection::sendLine(const std::string &line) {
    if (fd_ < 0) {
        return false;
    }

    std::string data = line;
    if (data.empty() || data.back() != '\n') {
        data.push_back('\n');
    }

    const char *buf = data.c_str();
    size_t total = 0;
    size_t to_send = data.size();

    while (total < to_send) {
        ssize_t n = ::send(fd_, buf + total, to_send - total, 0);
        if (n <= 0) {
            std::perror("send");
            return false;
        }
        total += static_cast<size_t>(n);
    }

    return true;
}

bool TcpConnection::recvLine(std::string &line) {
    line.clear();
    if (fd_ < 0) {
        return false;
    }

    char c = 0;
    while (true) {
        ssize_t n = ::recv(fd_, &c, 1, 0);
        if (n == 0) {
            return false;
        }
        if (n < 0) {
            std::perror("recv");
            return false;
        }
        if (c == '\n') {
            break;
        }
        line.push_back(c);
    }

    return true;
}

void TcpConnection::close() {
    if (fd_ >= 0) {
        ::shutdown(fd_, SHUT_RDWR);
        ::close(fd_);
        fd_ = -1;
    }
    if (listen_fd_ >= 0) {
        ::close(listen_fd_);
        listen_fd_ = -1;
    }
}
