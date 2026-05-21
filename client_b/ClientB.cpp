#include "ClientB.h"

#include "TcpConnection.h"
#include <chrono>
#include <cmath>

#include "Accel.h"

#include <accel.grpc.pb.h>
#include <grpcpp/create_channel.h>

static int64_t nowMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

double calc_distance(double x, double y, double z) {
    return sqrt(x * x + y * y + z * z);
}

ClientB::ClientB() : Client(log_dir) { }

void ClientB::run(const std::string &host, uint16_t port) {
    auto channel = grpc::CreateChannel(
        host + ":" + std::to_string(port),
        grpc::InsecureChannelCredentials()
    );
    auto stub = AccelerometerService::NewStub(channel);

    grpc::ClientContext ctx;
    ctx.AddMetadata("role", "B");

    auto stream = stub->StreamAccelData(&ctx);

    AccelPacket packet;
    while (stream->Read(&packet)) {
        AccelModule result;
        result.set_timestamp(packet.timestamp());
        result.set_module(calc_distance(packet.x(), packet.y(), packet.z()));

        if (!stream->Write(result)) {
            log("[B] send failed");
            break;
        }

        log("[B] got: " + packet.DebugString());
    }

    stream->WritesDone();

    auto status = stream->Finish();
    if (!status.ok()) {
        log(std::string("[B] gRPC error: ") + status.error_message());
    }

    log("[B] finished");
}