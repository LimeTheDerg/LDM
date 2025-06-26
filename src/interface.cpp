#include <fstream>
#include <iostream>
#include <bits/this_thread_sleep.h>

#include "lib/json/json.hpp"
#include "lib/util/helper_functions.hpp"

using std::cout;
using std::cerr;
using std::string;

using json = nlohmann::json;

int main(int argc, char *argv[]) {

    // Checks to see if an adequate number of arguments were given
    if (argc > 3 || argc < 2) {
        cerr << "----------ERROR----------" << "\n" <<
                "Improper number of arguments, use command 'ldmi help' for proper usage." << "\n";
        return 1;
    }

    // Initialize the terms of the operation
    const string operation = argv[1];
    const string target = argc > 2 ? argv[2] : "";
    json cache;

    try {
        cache = read_cache(); // Read from cache
    } catch (const std::exception &e) {
        cout << "Error reading from cache file: " << e.what() << "\n";
    }


    if (operation == "help" && argc == 2) {
        // TODO: Create help.txt file and read from it here.
        return 0;
    }

    if (operation == "list" && argc == 2) {
        cout << "Available daemons:" << "\n";
        for (int i = 0; i < cache.size(); i++) {
            const string str = cache["daemons"][i]["name"];
            cout << str << "\n";
        }
        return 0;
    }

    if (operation == "start" && argc == 3) {
        int daemon_id = 0;
        cout << "Starting daemon: " << target << "..." << "\n";
        // Search cache if daemon is available, error if not.
        bool isValid = false;
        for (int i = 0; i < cache.size(); i++) {
            if (cache["daemons"][i]["name"] == target) {
                isValid = true;
                daemon_id = i;
            }
        }
        if (!isValid) {
            cerr << "Daemon not found. Run ldmi list for a list of available daemons." << "\n";
            return 1;
        }
        // Ensuring child process spawning works correctly.
        program_state process_status = fork_execv_parent(target, cache);
        if (process_status == ERROR) {
            cerr << "Error starting daemon.\n";
            return 1;
        }
        //cache["daemons"][daemon_id]["status"] = "active";
        return 0;
    }

    return 0;
}