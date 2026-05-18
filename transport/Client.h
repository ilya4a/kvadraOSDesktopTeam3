
#ifndef KVADRAOSDESKTOPTEAM3_CLIENT_H
#define KVADRAOSDESKTOPTEAM3_CLIENT_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <mutex>

class Client {

    const std::filesystem::path log_client_name = "client.log";
    std::filesystem::path log_client_dir;
    std::ofstream logClient;

    std::mutex m;

protected:
	Client(std::filesystem::path client_dir) : log_client_dir(client_dir) {
		std::error_code ec;
		std::filesystem::create_directories(log_client_dir, ec);

	    if (ec) {
	        std::cerr << "Client: failed to create directories " << log_client_dir
                      << ": " << ec.message() << std::endl;
	    }

	    logClient = std::ofstream( log_client_dir / log_client_name);

        if (!logClient) std::cerr << "Client: couldn't open a " << log_client_name << std::endl;
    }

    void log(std::string const& s) {

	  std::scoped_lock<std::mutex> lock(m);

        if (logClient) {
            logClient << s << '\n';
        }else {
            std::cerr << "log: couldn't open a " << log_client_dir/log_client_name << std::endl;
        }
    }

public:
  virtual void run(const std::string& host, uint16_t port) = 0;
  virtual ~Client() = default;

};

#endif // KVADRAOSDESKTOPTEAM3_CLIENT_H
