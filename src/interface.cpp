#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc > 2) {
        cerr << "----------ERROR----------" << endl <<
                "Too many arguments given, please refer to documentation for proper usage." << endl;
        return 1;
    }

    return 0;
}