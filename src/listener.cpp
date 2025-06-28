#include <iostream>
#include <bits/this_thread_sleep.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "lib/util/helper_functions.hpp"

using std::string;
using std::cout;

int main() {
    const string fifo = find_process_path() + "/fifo";
    const char* fifo_c = fifo.c_str();
    char buffer[256];
    cout << "Opening daemon listener...";
    while (true) {
        int fifo_stream = open(fifo_c, O_RDONLY);
        if (fifo_stream < 0) {
            std::cerr << "No daemons are currently active." << "\n";
            return 0;
        }
        read(fifo_stream, buffer, 256);
        cout << buffer << "\n";
        close(fifo_stream);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}