#include <csignal>
#include <iostream>
#include <bits/this_thread_sleep.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "lib/util/helper_functions.hpp"

using std::string;
using std::cout;

int main() {
    std::signal(SIGINT, listener_clean_exit);  // Register signal handler

    // Create fifo
    const string fifo = find_process_path() + "/fifo";
    mkfifo(fifo.c_str(), 0666);
    const char* fifo_c = fifo.c_str();
    char buffer[512];

    cout << "Opening daemon listener..." << "\n";
    cout << "Press Ctrl+C to quit" << "\n";

    while (true) {
        const int fifo_stream = open(fifo_c, O_RDONLY);
        read(fifo_stream, buffer, 512);
        cout << buffer;
        close(fifo_stream);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}