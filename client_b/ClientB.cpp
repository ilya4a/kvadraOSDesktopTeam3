
#include "ClientB.h"

#include "TcpConnection.h"
#include <chrono>
#include <cmath>

#include "Accel.h"


static int64_t nowMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
    ).count();
}

double calc_distance(double x, double y, double z ) {
    return sqrt(x*x + y*y + z*z);
}

ClientB::ClientB() : Client(log_dir) {}

void ClientB::run(const std::string& host, uint16_t port) {

    TcpConnection conn(-1);

    if (!conn.connectTo(host, port)) {
        log("[B] connect failed");
        return;
    }

    conn.sendLine(ROLE_B);
    log("[B] connected");

    while (true) {
        std::string response;
        if (!conn.recvLine(response)) {
            log("[B] recv failed");
            break;
        }

        AccelData data = AccelData::from_json(response);

        double dis = calc_distance(data.x, data.y, data.z);

        AccelResult res(data.timestamp, dis);

        if (!conn.sendLine(res.to_json().dump())) {
            log("[B] send failed");
            break;
        }

        log("[B] got: " + response);
    }
}