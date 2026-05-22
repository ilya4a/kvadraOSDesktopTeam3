#include "Server.h"

#include <cmath>
#include <iostream>

Server::Server(uint16_t port, std::string b_host, uint16_t b_port)
    : port_(port),
      service_(*this),
      b_target_(std::move(b_host) + ":" + std::to_string(b_port)),
      b_channel_(grpc::CreateChannel(b_target_, grpc::InsecureChannelCredentials())),
      b_stub_(AccelerometerService::NewStub(b_channel_)) {}

bool Server::isDuplicates(const AccelPacket &prev, const AccelPacket &cur) {
    return std::abs(prev.x() - cur.x()) < DUPLICATES_ACCURACY
        && std::abs(prev.y() - cur.y()) < DUPLICATES_ACCURACY
        && std::abs(prev.z() - cur.z()) < DUPLICATES_ACCURACY;
}

grpc::Status Server::ServiceImpl::StreamAccelData(
    grpc::ServerContext * context,
    grpc::ServerReaderWriter<AccelModule, AccelPacket> *stream) {

    grpc::ClientContext bctx;
    auto bstream = owner_.b_stub_->StreamAccelData(&bctx);

    if (!bstream) {
        return grpc::Status(grpc::StatusCode::UNAVAILABLE, "cannot connect to B");
    }

    AccelPacket prev;
    bool has_prev = false;
    AccelPacket cur;

    while (stream->Read(&cur)) {
        if (has_prev && owner_.isDuplicates(prev, cur)) {
            continue;
        }

        has_prev = true;
        prev = cur;

        if (!bstream->Write(cur)) {
            return grpc::Status(grpc::StatusCode::UNAVAILABLE, "write to B failed");
        }

        AccelModule out;
        if (!bstream->Read(&out)) {
            return grpc::Status(grpc::StatusCode::UNAVAILABLE, "read from B failed");
        }

        if (!stream->Write(out)) {
            return grpc::Status(grpc::StatusCode::UNAVAILABLE, "write to A failed");
        }
    }

    bstream->WritesDone();
    return bstream->Finish();
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