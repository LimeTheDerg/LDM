#include <chrono>
#include <cmath>
#include <iostream>
#include <fstream>
#include <thread>
#include "daemon_api.hpp"

using namespace std;

int main(const int argc, char* argv[]) {
    daemonize();

    if (argc != 1) {
        return 1;
    }

    const int warn_thresh = atoi(argv[0]);

    while (true) {
        ifstream cpu_file;
        cpu_file.open("/sys/class/thermal/thermal_zone0/temp", ios::in); // Opens the file that contains temperature information for Linux
        float temp;
        if (cpu_file >> temp) {
            const float temp_c = temp/1000; // Divide by 1000 because the temp file expresses the temperature as (actual temp * 1000)
            if (temp_c > warn_thresh) {
                // '\n' is mandatory for clean printing, the double '\n' is optional since it allows the user to differentiate between daemons
                string warning_message = "cpu-temperature-monitor - [WARNING] - CPU temperature has exceeded " + to_string(round(warn_thresh)) + " degrees C!" + "\n" +
                                         "cpu-temperature-monitor - [INFO] - Current CPU temperature is: " + to_string(round(temp_c)) + " degrees C." + "\n\n";
                write_fifo(warning_message);
            }
        } else {
            return 1;
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
}
