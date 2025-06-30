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
    const string fifo = find_bin_path() + "/fifo";
    mkfifo(fifo.c_str(), 0666);

    cout << "Opening daemon listener..." << "\n";
    cout << "Press Ctrl+C to quit" << "\n";

    const int fifo_stream = open(fifo.c_str(), O_RDONLY | O_NONBLOCK);

    while (true) {
        char buffer[4096]; // Create the buffer
        const ssize_t bytes_read = read(fifo_stream, buffer, 4096); // Read from the fifo
        buffer[bytes_read] = '\0'; // Null terminate
        if (bytes_read > 0) {
            cout << buffer << std::flush; // Flush is mandatory, printing will not work without it
        }
    }
    return 0;
}