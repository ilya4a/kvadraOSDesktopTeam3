#ifndef KVADRAOSDESKTOPTEAM3_BSERVER_H
#define KVADRAOSDESKTOPTEAM3_BSERVER_H

#include <memory>

#include "accel.grpc.pb.h"
#include <grpcpp/grpcpp.h>

#include <Client.h>

class ClientB final : public Client {
    class ServiceImpl final : public AccelerometerService::Service {
        ClientB &owner_;

      public:
        ServiceImpl(ClientB &owner);
        grpc::Status StreamAccelData(
            grpc::ServerContext *context,
            grpc::ServerReaderWriter<AccelModule, AccelPacket> *stream
        ) override;
    };

    uint16_t port_;
    ServiceImpl service_;
    std::unique_ptr<grpc::Server> server_;

  public:
    void run(const std::string &host, uint16_t port) override;
    explicit ClientB(uint16_t port);
};

#endif // KVADRAOSDESKTOPTEAM3_BSERVER_H
