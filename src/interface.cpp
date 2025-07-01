#include <fstream>
#include <iostream>

#include "lib/json/json.hpp"
#include "lib/util/helper_functions.hpp"

using std::cout;
using std::cerr;
using std::string;

using nlohmann::json;

int main(int argc, char *argv[]) {

    chdir(find_bin_path().c_str()); // Set working directory to solve inconsistencies
    clear_kill_file(); // Clear the kill file so processes don't stop unnecessarily

    // Checks to see if an adequate number of arguments were given
    if (argc > 3 || argc < 2) {
        cerr << "----------ERROR----------" << "\n" <<
                "Improper number of arguments, use command 'ldmi help' for proper usage." << "\n";
        return 1;
    }

    // Initialize the terms of the operation
    const string operation = argv[1];
    const string target = argc > 2 ? argv[2] : "";
    json cache = read_cache(); // Read from cache


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
                // if (cache["daemons"][i]["status"] == "active") {
                //     cout << "Daemon is already active." << "\n";
                //     return 0;
                // }
            }
        }
        if (!isValid) {
            cerr << "Daemon not found. Run 'ldmi list' for a list of available daemons." << "\n";
            return 1;
        }
        // Ensuring child process spawning works correctly.
        const program_state process_status = fork_execv_parent(target, cache);
        if (process_status == ERROR) {
            cerr << "Error starting daemon.\n";
            return 1;
        }

        if (process_status == PARENT) {
            cache["daemons"][daemon_id]["status"] = "active";
            try {
                write_cache(cache);
            } catch (const std::exception &e) {
                cout << "Error opening cache file: " << e.what() << "\n";
            }
            cout << "Daemon started successfully." << "\n";
            return 0;
        }
        return 0;
    }

    if (operation == "stop" && argc == 3) {
        for (int i = 0; i < cache.size(); i++) {
            if (cache["daemons"][i]["name"] == target) {
                if (cache["daemons"][i]["status"] == "inactive") {
                    cout << "Daemon is already stopped." << "\n";
                    return 0;
                }
                if (cache["daemons"][i]["status"] == "active") {
                    write_kill_file(cache["daemons"][i]["bin"]);
                    cache["daemons"][i]["status"] = "inactive";
                    write_cache(cache);
                    cout << "Stopping daemon..." << "\n";
                    return 0;
                }
            } else {
                cout << "Daemon not found." << "\n";
                return 1;
            }
        }
    }

    if (operation == "config" && argc == 3) {
        for (int i = 0; i < cache.size(); i++) {
            if (cache["daemons"][i]["name"] == target) {
                cout << "Enter your configuration here, here is the specification:" << "\n";
                const string conf_msg = cache["daemons"][i]["conf_msg"];
                cout << conf_msg << "\n";
                string arguments;
                std::cin >> arguments;
                cache["daemons"][i]["args"] = arguments;
                write_cache(cache);
                cout << "Configuration set. Stopping." << "\n";
                return 0;
            }
        }
    }

    cerr << "Invalid usage, wrong arguments. Run 'ldmi help' for proper usage" << "\n";

    return 0;
}