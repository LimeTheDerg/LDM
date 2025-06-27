#include <iostream>
#include <bits/this_thread_sleep.h>
#include <sys/stat.h>

#include "lib/util/helper_functions.hpp"

using std::string;

int main() {
    const string fifo = find_process_path() + "/fifo";
    const char* fifo_c = fifo.c_str();
    mkfifo(fifo_c, 0666);
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}