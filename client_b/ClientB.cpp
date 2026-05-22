#include "ClientB.h"

#include <cmath>
#include <stdexcept>

ClientB::ServiceImpl::ServiceImpl(ClientB &owner) : owner_(owner) { }

grpc::Status ClientB::ServiceImpl::StreamAccelData(
    grpc::ServerContext *,
    grpc::ServerReaderWriter<AccelModule, AccelPacket> *stream
) {
    AccelPacket packet;
    while (stream->Read(&packet)) {
        AccelModule result;
        result.set_timestamp(packet.timestamp());
        result.set_module(std::sqrt(packet.x() * packet.x() + packet.y() * packet.y() + packet.z() * packet.z()));
        stream->Write(result);

        owner_.log(
            "[B] ts: = " + std::to_string(result.timestamp()) + " mod: " + std::to_string(result.module())
        );
    }
    return grpc::Status::OK;
}

ClientB::ClientB(uint16_t port) : port_(port), service_(*this), Client("logs/client_b") {
    std::cout << "B started on port " << port_ << std::endl;
}

void ClientB::run(const std::string &host, uint16_t port) {
    grpc::ServerBuilder builder;
    builder.AddListeningPort(host + ":" + std::to_string(port_), grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);

    server_ = builder.BuildAndStart();
    if (!server_) {
        throw std::runtime_error("failed to start B server");
    }

    server_->Wait();
}
