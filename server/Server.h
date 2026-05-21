#ifndef KVADRAOSDESKTOPTEAM3_SERVER_H
#define KVADRAOSDESKTOPTEAM3_SERVER_H

#include <memory>

#include <grpcpp/grpcpp.h>
#include "accel.pb.h"
#include "accel.grpc.pb.h"

class Server {
  static constexpr double DUPLICATES_ACCURACY = 1e-4;

  class ServiceImpl final : public AccelerometerService::Service {
    Server &owner_;

  public:
    explicit ServiceImpl(Server &owner);

    grpc::Status StreamAccelData(
        grpc::ServerContext *context,
        grpc::ServerReaderWriter<AccelModule, AccelPacket> *stream) override;
  };

  uint16_t port_;
  ServiceImpl service_;
  std::unique_ptr<grpc::Server> grpc_server_;

  static bool isDuplicates(const AccelPacket &prev, const AccelPacket &cur);

public:
  explicit Server(uint16_t port);
  void run();
};

#endif // KVADRAOSDESKTOPTEAM3_SERVER_H