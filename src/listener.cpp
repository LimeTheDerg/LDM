#include <iostream>
#include <sys/stat.h>

#include "lib/util/helper_functions.hpp"

int main() {
    const char* fifo = find_process_path().c_str();
    mkfifo(fifo, 0666);
    while (true) {

    }
    return 0;
}