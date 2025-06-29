#include <iostream>
#include <bits/this_thread_sleep.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "lib/util/helper_functions.hpp"

using std::string;
using std::cout;

int main() {
    init_termios();

    // Create fifo
    const string fifo = find_process_path() + "/fifo";
    mkfifo(fifo.c_str(), 0666);
    const char* fifo_c = fifo.c_str();
    char buffer[512];

    cout << "Opening daemon listener..." << "\n";
    cout << "Press F10 to quit" << "\n";

    char esc_seq[6] = {0};
    while (true) {

        if (is_f10_pressed()) {
            unlink(fifo.c_str());
            std::cout << "Quitting..." << "\n";
            break;
        }

        const int fifo_stream = open(fifo_c, O_RDONLY);
        read(fifo_stream, buffer, 512);
        cout << buffer;
        close(fifo_stream);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}