#include <chrono>
#include <thread>

#include "daemon_api.hpp"

int main(int argc, char* argv[]) {
    daemonize(argv[0]);

    if (argc != 3) {
        daemon_api_log("[ERROR] - INVALID ARGUMENT OR CONFIGURATION: " + bin_name);
    }
    int time_before_clear = 0;
    try {
        time_before_clear = std::stoi(argv[1]);
    } catch (const std::invalid_argument &e) {
        daemon_api_log("[ERROR] - INVALID ARGUMENT OR CONFIGURATION: " + bin_name);
    }

    while (true) {
        std::string content = argv[2];
        for (int i = 1; i <= time_before_clear; i++) {
            read_kill_file();
            std::string send_file_content = read_send_file();
            if (!send_file_content.empty()) {
                if (send_file_content == "view") {
                    write_fifo("vault - [INFO] - The vault's current contents are: " + content + "\n");
                } else {
                    content = send_file_content;
                    i = 1;
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}