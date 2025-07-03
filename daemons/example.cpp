/**
 * Very simple daemon example to get you started.
 * This daemon will simply send whatever is in its config to the listener every 10 seconds.
 * It will also read whatever is sent to it, and
 * prioritize printing that to the listener.
 */
#include <chrono>
#include <thread>

#include "daemon_api.hpp"


// The arguments are a vector of the configuration in daemon_cache.json that has been space separated, using something like
// "I am space separated" as a single argument is not supported yet.
int main(int argc, const char *argv[]) {

    daemonize(argv[0]); // This is necessary for the daemon to work properly, and must be placed at the top of your entry point, in this case, main()
                        // You must also pass argv[0] as an argument to the function

    // Checks if the argument count is right, even though there is only one config value, the argc is 2, this is because of how C++ treats
    if (argc != 2) {

        // This is the logging function, it appends the arguments of the function to log
        daemon_api_log("[ERROR] - INVALID ARGUMENT OR CONFIGURATION: " + bin_name); // bin_name is a variable in the daemon_api.hpp file that is the name of the daemon,
                                                                                            // this variable is meant for the API itself, but you can use it like this if you like, but DO NOT MODIFY IT
        return 1;
    }

    // Daemons would probably be running on infinite loops like this
    while (true) {

        read_kill_file(); // This is a very important function, this reads from the kill file and will kile the daemon accordingly,
                          // failing to put this in your daemon will make it unable to terminate

        std::string sendfile_contents = read_send_file(); // This is the function for reading from sendfile. It returns a string of whatever was in the file, note that the file WILL be truncated on a successful read

        if (sendfile_contents.empty()) {
            std::stringstream message;
            message << "example - [INFO] - I am an example!: " << argv[1] << "\n\n";
            write_fifo(message.str()); // This function writes whatever is in its argument to the listener. Note that the daemon will pause until a listener instance actually opens due to the nature of FIFOs
        }
        else {
            std::stringstream message;
            message << "example - [INFO] - I am an example!: " << sendfile_contents << "\n\n";
            write_fifo(message.str());
        }

        std::this_thread::sleep_for(std::chrono::seconds(10)); // Putting a sleep is HIGHLY recommended at the end of every loop so you don't use 100% of CPU
    }
}