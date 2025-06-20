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
                "Improper number of arguments, use command 'ldmi help' for proper usage." << endl;
        return 1;
    }

    // Initialize the terms of the operation
    const string operation = argv[1];
    const string target = argc > 2 ? argv[2] : "";
    json cache = read_cache(); // Read from cache


    if (operation == "help" && argc == 2) {
        // TODO: Create help.txt file and read from it here.
    }

    if (operation == "list" && argc == 2) {
        cout << "Available daemons:" << endl;
        for (int i = 0; i < cache.size(); i++) {
            const string str = cache["daemons"][i]["name"];
            cout << str << endl;
        }
        return 0;
    }
    return 0;
}