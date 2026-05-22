#include "Server.h"

#include <cmath>
#include <iostream>

void Server::shutdown() {
    if (grpc_server_) {
        grpc_server_->Shutdown();
    }
}

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
    grpc::ServerContext *context,
    grpc::ServerReaderWriter<AccelModule, AccelPacket> *stream)
{
    std::cout << "[S] new stream from A, opening stream to B at "
              << owner_.b_target_ << std::endl;

    grpc::ClientContext bctx;
    auto bstream = owner_.b_stub_->StreamAccelData(&bctx);
    if (!bstream) {
        std::cerr << "[S] failed to create stream to B" << std::endl;
        return grpc::Status(grpc::StatusCode::UNAVAILABLE, "cannot connect to B");
    }
    std::cout << "[S] connected to B, starting relay" << std::endl;

    AccelPacket prev;
    bool has_prev = false;
    AccelPacket cur;
    int relayed_count = 0;

    while (stream->Read(&cur)) {
        if (has_prev && owner_.isDuplicates(prev, cur)) {
            continue;
        }
        has_prev = true;
        prev = cur;

        if (!bstream->Write(cur)) {
            std::cerr << "[S] write to B failed" << std::endl;
            return grpc::Status(grpc::StatusCode::UNAVAILABLE, "write to B failed");
        }

        AccelModule out;
        if (!bstream->Read(&out)) {
            std::cerr << "[S] read from B failed" << std::endl;
            return grpc::Status(grpc::StatusCode::UNAVAILABLE, "read from B failed");
        }

        if (!stream->Write(out)) {
            std::cerr << "[S] write to A failed" << std::endl;
            return grpc::Status(grpc::StatusCode::UNAVAILABLE, "write to A failed");
        }
        relayed_count++;
    }

    bstream->WritesDone();
    grpc::Status bstatus = bstream->Finish();
    std::cout << "[S] stream finished, relayed " << relayed_count
              << " packets, B status: " << bstatus.error_message() << std::endl;
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