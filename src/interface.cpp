#include <fstream>
#include <iostream>
#include "lib/json/json.hpp"
#include "lib/util/helper_functions.hpp"

using namespace std;
using json = nlohmann::json;

int main(int argc, char *argv[]) {

    // Checks to see if an adequate number of arguments were given
    if (argc > 3 || argc < 2) {
        cerr << "----------ERROR----------" << endl <<
                "Improper number of arguments, use command 'ldmi help' to learn proper usage." << endl;
        return 1;
    }

    // Initialize the terms of the operation
    const string operation = argv[1];
    const string target = argv[2];

    if (operation == "help") {
        // TODO: Create help.txt file and read from it here.
    }

    if (operation == "start") {

        json cache = read_cache(); // Read from cache

        for (int i = 0; i < cache.size(); i++) {
            cout << cache["daemons"][i]["conf_msg"] << endl;
        }
    }

    return 0;
}