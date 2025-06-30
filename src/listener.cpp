#include <csignal>
#include <iostream>
#include <bits/this_thread_sleep.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "lib/util/helper_functions.hpp"

using std::string;
using std::cout;

int main() {
    std::signal(SIGINT | SIGTERM, listener_clean_exit);  // Register signal handler

    // Create fifo
    const string fifo = find_process_path() + "/fifo";
    mkfifo(fifo.c_str(), 0666);
    char buffer[512];

    cout << "Opening daemon listener..." << "\n";
    cout << "Press Ctrl+C to quit" << "\n";

    const int fifo_stream = open(fifo.c_str(), O_RDONLY);

    while (true) {
        read(fifo_stream, buffer, 512);
        cout << buffer << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}