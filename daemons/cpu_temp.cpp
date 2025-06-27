#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>

using namespace std;

[[noreturn]] int main(const int argc, char* argv[]) {

    if (argc != 1) {
        cerr << "Error reading from cache, cache may be malformed or tampered with." << "\n";
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
                std::cout << "cpu-temperature-monitor - [WARNING] - CPU temperature has exceeded " << warn_thresh << " degrees C!" << "\n";
                std::cout << "cpu-temperature-monitor - [INFO] - Current CPU temperature is: " << temp_c << " degrees C." << "\n";
            }
        } else {
            cerr << "Failed to read temperature." << endl; // In case of error, I don't want to see this...
        }
        this_thread::sleep_for(chrono::seconds(3)); // Wait for 3 seconds
    }
}
