#ifndef KVADRAOSDESKTOPTEAM3_SERVER_H
#define KVADRAOSDESKTOPTEAM3_SERVER_H

#include <memory>

#include "accel.grpc.pb.h"
#include "accel.pb.h"
#include <grpcpp/grpcpp.h>
#include <string>

class Server {
    static constexpr double DUPLICATES_ACCURACY = 1e-4;

    class ServiceImpl final : public AccelerometerService::Service {
        Server &owner_;

      public:
        explicit ServiceImpl(Server &owner) : owner_(owner) { }

        grpc::Status StreamAccelData(
            grpc::ServerContext *context,
            grpc::ServerReaderWriter<AccelModule, AccelPacket> *stream
        ) override;
    };

    uint16_t port_;
    ServiceImpl service_;
    std::unique_ptr<grpc::Server> grpc_server_;

    std::string b_target_;
    std::shared_ptr<grpc::Channel> b_channel_;
    std::unique_ptr<AccelerometerService::Stub> b_stub_;

    static bool isDuplicates(const AccelPacket &prev, const AccelPacket &cur);

  public:
    Server(uint16_t port, std::string b_host, uint16_t b_port);
    void run();
};

#endif // KVADRAOSDESKTOPTEAM3_SERVER_H
