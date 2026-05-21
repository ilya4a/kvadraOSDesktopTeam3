#include "Server.h"
#include <cmath>
#include <iostream>

Server::Server(uint16_t port)
    : port_(port), service_(*this) {}

bool Server::isDuplicates(const AccelPacket &prev, const AccelPacket &cur) {
    return std::abs(prev.x() - cur.x()) < DUPLICATES_ACCURACY
        && std::abs(prev.y() - cur.y()) < DUPLICATES_ACCURACY
        && std::abs(prev.z() - cur.z()) < DUPLICATES_ACCURACY;
}

Server::ServiceImpl::ServiceImpl(Server &owner)  : owner_(owner) {};


grpc::Status Server::ServiceImpl::StreamAccelData(
    grpc::ServerContext * context,
    grpc::ServerReaderWriter<AccelModule, AccelPacket> *stream) {

    AccelPacket prev;
    bool has_prev = false;
    AccelPacket cur;

    while (stream->Read(&cur)) {
        if (has_prev && owner_.isDuplicates(prev, cur)) {
            continue;
        }

        has_prev = true;
        prev = cur;

        AccelModule out;
        out.set_timestamp(cur.timestamp());
        out.set_module(std::sqrt(cur.x() * cur.x() + cur.y() * cur.y() + cur.z() * cur.z()));

        stream->Write(out);
    }

    return grpc::Status::OK;
}

void Server::run() {
    const std::string address = "0.0.0.0:" + std::to_string(port_);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);

    grpc_server_ = builder.BuildAndStart();
    if (!grpc_server_) {
        std::cerr << "[S] failed to start gRPC server\n";
        return;
    }

    std::cout << "[S] listening on " << address << "\n";
    grpc_server_->Wait();
}
