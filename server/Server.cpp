#include "Server.h"
//
// #include <iostream>
//
// std::string Server::readRole(TcpConnection &c) {
//     std::string line;
//     if (!c.recvLine(line)) {
//         return "";
//     }
//     return line;
// }
//
// void Server::waitForClients() {
//     connA.reset();
//     connB.reset();
//
//     while (!connA || !connB) {
//         auto client = listener.acceptClient();
//         if (!client) {
//             continue;
//         }
//         std::string role = Server::readRole(*client);
//         if (role == ROLE_A && !connA) {
//             connA = std::move(client);
//             std::cout << "[S] client A connected\n";
//         } else if (role == ROLE_B && !connB) {
//             connB = std::move(client);
//             std::cout << "[S] client B connected\n";
//         } else {
//             std::cerr << "[S] unknown role or duplicate\n";
//         }
//     }
// }
//
// Server::Server(uint16_t port) : port(port), listener(-1) {
//     if (!listener.bindAndListen(port)) {
//         std::cerr << "[S] listen failed\n";
//         return;
//     }
//
//     std::cout << "[S] listening on port " << port << "\n";
//
//     waitForClients();
// }
//
// bool Server::isDuplicates(AccelData exResult, AccelData newResult) {
//     return (
//         std::abs(exResult.x - newResult.x) < DUPLICATES_ACCURACY
//         && std::abs(exResult.y - newResult.y) < DUPLICATES_ACCURACY
//         && std::abs(exResult.z - newResult.z) < DUPLICATES_ACCURACY
//     );
// }
//
// void Server::run() {
//     AccelData exResult(0, 0, 0, 0);
//
//     while (true) {
//         waitForClients();
//         while (true) {
//             std::string packet;
//             if (!connA->recvLine(packet)) {
//                 std::cerr << "[S] A disconnected\n";
//                 break;
//             }
//             // std::cout << "[S] from A: " << packet << "\n";
//             AccelData newResult = AccelData::from_json(packet);
//
//             if (isDuplicates(exResult, newResult)) {
//                 exResult = newResult;
//                 if (!connA->sendLine(AccelResult(newResult.timestamp, 0).to_json().dump())) {
//                     std::cerr << "[S] send to A failed\n";
//                     break;
//                 }
//                 continue;
//             }
//             if (!connB->sendLine(packet)) {
//                 std::cerr << "[S] send to B failed\n";
//                 break;
//             }
//             std::string result;
//             if (!connB->recvLine(result)) {
//                 std::cerr << "[S] B disconnected\n";
//                 break;
//             }
//             // std::cout << "[S] from B: " << result << "\n";
//             if (!connA->sendLine(result)) {
//                 std::cerr << "[S] send to A failed\n";
//                 break;
//             }
//         }
//     }
// }
