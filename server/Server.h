
#ifndef KVADRAOSDESKTOPTEAM3_SERVER_H
#define KVADRAOSDESKTOPTEAM3_SERVER_H
#include <Accel.h>


#include "TcpConnection.h"

#include <optional>
#include <string>


class Server {
  TcpConnection listener;

  std::optional<TcpConnection> connA;
  std::optional<TcpConnection> connB;

  uint16_t port;

  static std::string readRole(TcpConnection& c);

public:
  Server(uint16_t port);

  void run();

};


#endif // KVADRAOSDESKTOPTEAM3_SERVER_H
