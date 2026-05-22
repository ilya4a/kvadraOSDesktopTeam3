#include "ClientA.h"

#include <accel.grpc.pb.h>
#include <atomic>
#include <chrono>
#include <cmath>
#include <fstream>
#include <grpcpp/create_channel.h>
#include <iostream>
#include <thread>

using namespace std::chrono;
using Clock = steady_clock;

static int64_t nowMs() {
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

ClientA::ClientA() : Client(log_dir) {
    logModule = std::ofstream(log_mod_dir / log_mod_name, std::ios::app);
    if (!logModule) {
        std::cerr << "couldn't open a " << log_mod_dir / log_mod_name << std::endl;
    }
}

void ClientA::run(const std::string &host, uint16_t port) {
    auto channel = grpc::CreateChannel(host + ":" + std::to_string(port), grpc::InsecureChannelCredentials());
    auto stub = AccelerometerService::NewStub(channel);

    grpc::ClientContext ctx;
    ctx.AddMetadata("role", "A");

    auto stream = stub->StreamAccelData(&ctx);

    if (!stream) {
        std::cerr << "[A] failed to create stream" << std::endl;
        return;
    }
    std::cout << "[A] connected to server, starting sensor stream" << std::endl;

    std::atomic<bool> running { true };

    std::thread sender([&]() {
        auto nextSendTime = Clock::now();
        int i = 0;

        while (running) {
            AccelPacket msg;
            msg.set_timestamp(nowMs());
            msg.set_x(std::sin(i * 0.1));
            msg.set_y(0.0);
            msg.set_z(0.0);
            ++i;

            if (!stream->Write(msg)) {
                running = false;
                break;
            }

            log(msg.DebugString());

            nextSendTime += SEND_INTERVAL;
            std::this_thread::sleep_until(nextSendTime);
        }

        stream->WritesDone();
    });

    AccelModule response;
    while (running && stream->Read(&response)) {
        while (running && stream->Read(&response)) {
            if (logModule.is_open()) {
                logModule << response.timestamp() << " " << response.module() << std::endl;
                logModule.flush();
            }

            log("[A]: receive module = " + std::to_string(response.module())
                + " ts: " + std::to_string(response.timestamp()));
        }
    }

    running = false;
    sender.join();

    auto status = stream->Finish();
    if (!status.ok()) {
        std::cerr << "[A] gRPC error: " << status.error_message() << std::endl;
    }
}
