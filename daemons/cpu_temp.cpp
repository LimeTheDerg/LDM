#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>

using namespace std;

[[noreturn]] int main() {
    while (true) {
        ifstream cpu_file;
        cpu_file.open("/sys/class/thermal/thermal_zone0/temp", ios::in); // Opens the file that contains temperature information for Linux
        // Makes sure there are no errors
        float temp;
        if (cpu_file >> temp) {
            const float temp_c = temp/1000; // Divide by 1000 because the temp file expresses the temperature as (actual temp * 1000)
            cout << temp_c << endl;
        } else {
            cerr << "Failed to read temperature." << endl; // In case of error, I don't want to see this...
        }
        this_thread::sleep_for(chrono::seconds(3)); // Wait for 3 seconds
    }
}
