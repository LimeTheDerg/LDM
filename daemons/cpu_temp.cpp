#include <chrono>
#include <fstream>
#include <thread>
#include "daemon_api.h"

using namespace std;

int main(const int argc, char* argv[]) {
    daemonize(argv[0]);

    if (argc != 2) {
        return 1;
    }
    int warn_thresh = 0;
    try {
        warn_thresh = stoi(argv[1]);
    } catch (const invalid_argument &e) {
        daemon_api_log("[ERROR] - INVALID ARGUMENT OR CONFIGURATION: " + bin_name);
    }

    while (true) {
        read_kill_file();
        ifstream cpu_file;
        cpu_file.open("/sys/class/thermal/thermal_zone0/temp", ios::in); // Opens the file that contains temperature information for Linux
        int temp;
        if (cpu_file >> temp) {
            const int temp_c = temp/1000; // Divide by 1000 because the temp file expresses the temperature as (actual temp * 1000)
            if (temp_c > warn_thresh) {
                // '\n' is mandatory for clean printing, the double '\n' is optional since it allows the user to differentiate between daemons
                string warning_message = "cpu-temperature-monitor - [WARNING] - CPU temperature has exceeded " + to_string(warn_thresh) + " degrees C!" + "\n" +
                                         "cpu-temperature-monitor - [INFO] - Current CPU temperature is: " + to_string(temp_c) + " degrees C." + "\n\n";
                write_fifo(warning_message);
            }
        } else {
            daemon_api_log("[ERROR] - ERROR OPENING THERMAL FILE: " + bin_name);
            return 1;
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
}
