#ifndef KVADRAOSDESKTOPTEAM3_ACCELDATA_H
#define KVADRAOSDESKTOPTEAM3_ACCELDATA_H


#include <nlohmann/json.hpp>
#include <cstdint>

using json = nlohmann::json;

const std::string ROLE_A = "ROLE A";
const std::string ROLE_B = "ROLE B";


struct AccelData {
    int64_t timestamp;
    double x, y, z;

    AccelData(int64_t timestamp,  double x, double y,double z) : timestamp(timestamp), x(x), y(y), z(z){}

    json to_json() const {
        return json{{"ts", timestamp}, {"x", x}, {"y", y}, {"z", z}};
    }

    static AccelData from_json(const std::string& s) {
        auto j = json::parse(s);
        return {j["ts"], j["x"], j["y"], j["z"]};
    }

};

struct AccelResult {
    int64_t timestamp;
    double module;

    AccelResult(int64_t timestamp, double module) : timestamp(timestamp), module(module) {}

    json to_json() const {
        return json { {"ts", timestamp}, {"mod" ,  module} };
    };

    static AccelResult from_json(const std::string& s) {
        auto j = json::parse(s);
        return {j["ts"], j["mod"]};
    }

};


#endif // KVADRAOSDESKTOPTEAM3_ACCELDATA_H
