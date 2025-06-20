#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>

using namespace std;

int main() {
    while (true) {
        ifstream cpu_file;
        cpu_file.open("/sys/class/thermal/thermal_zone0/temp", ios::in);
        if (int temp; cpu_file >> temp) {
            const int temp_c = temp/1000;
            cout << temp_c << endl;
        } else {
            cerr << "Failed to read temperature." << endl;
        }
        this_thread::sleep_for(chrono::seconds(3));
    }
}
