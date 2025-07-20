# LDM
**LDM** is a lightweight daemon control and communication system written in C++. It lets you start, stop, and send messages to custom daemons using a unified CLI, with IPC handled via FIFOs or text-based signaling.

## Features

- Spawn and kill daemons at will with a CLI application
- Read messages sent by any active daemon through one listener CLI application
- Configure daemons to change behavior
- Send information to daemons that support sending
- No external libraries/dependencies (however, it does use the Nlohmann JSON single-include header file)
- Very lightweight, built with Arch Linux but will likely work on most distros
- Base installation comes with 2 daemons, vault and cpu-temperature-monitor
- A very easy-to-use, lightweight API to build your own daemons with ease

## Structure
LDM is split into two applications, one interface, and one listener. The interface is used to spawn, kill, configure, or send messages to daemons, while the listener listens to messages sent by any active daemons. For IPC, the project uses Linux FIFOs or simply text files to communicate.

The interface has a help command that goes over how to use the application in detail (including listener), just run this command:
```
ldmi help
```

## Installing
You have two options:
1. Use the installation script: **install.sh**, which will configure PATH for you.
2. Download the bin directory as a whole, and choose to configure PATH yourself or not.

Note: Going with option 2 and not setting PATH means you have to move to the directory of the binaries within the shell and run commands with a ./ at the start, for example:
```
./ldmi help
```

Both options are available in the release found here: https://github.com/LimeTheDerg/LDM/releases/tag/First-Release

## Example usage
### Here is an example interaction with LDM, with the listener and interface running at the same time:
Interacting with daemons through ldmi:
```
ldmi config vault
>>> <clear-vault-delay-seconds> <default-value>
300 empty
>>> Configuration set. Stopping.
ldmi start vault
>>> Starting daemon: vault...
>>> Daemon started successfully.
ldmi send vault
>>> Enter the message you would like to send:
view
ldmi stop vault
>>> Stopping daemon...
```
Listening to daemons through ldml:
```
ldml
>>> Opening daemon listener...
>>> Press Ctrl+C to quit
>>> vault - [INFO] - The vault's current contents are: empty
>>>
```

## Daemon API documentation
To make it as easy as possible to make your own daemons, I have written a small API compatible with LDM. It contains some functions to simplify the process of creating a daemon significantly.

You can figure out the functionality by reading the comments in this file, which is the entire API. To use the API, all you have to do is include it. All the functions are commented and very self-explanatory.

Header:
https://github.com/LimeTheDerg/LDM/blob/main/daemons/daemon_api.h

Source:
https://github.com/LimeTheDerg/LDM/blob/main/daemons/daemon_api.cpp

You will need to create a cache entry yourself by modifying the daemon_cache.json file, read on for more details.

### Here is a quick tutorial on what you should do:
This is a quick and easy example daemon to get you started: Here's a link to the file: https://github.com/LimeTheDerg/LDM/blob/main/daemons/example.cpp
```cpp
/**
 * Very simple daemon example to get you started.
 * This daemon will simply send whatever is in its config to the listener every 10 seconds.
 * It will also read whatever is sent to it, and
 * prioritize printing that to the listener.
 */
#include <chrono>
#include <thread>
#include <sstream>

#include "daemon_api.h"

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

        read_kill_file(); // This is a very important function, this reads from the kill file and will kill the daemon accordingly,
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
```

Once you have written your daemon, you will have to **manually** create a cache entry like this:
```
{
  "daemons": [
    {
      "args": "example",
      "bin": "example",
      "conf_msg": "<timed-message>",
      "help": "\nDESCRIPTION:\nAn example daemon to get users started.\nCONFIG:\n<timed-message>\nThe timed message argument is the message the daemon will send every 10 seconds\nSENDING:\nAny information sent to this daemon will be printed at the next message send cycle.\n",
      "name": "example",
      "status": "inactive"
    }
  ]
}
```

You must create a cache entry like this. You may leave `args` empty as the user can just config it through the interface, but adding a default value is not a bad idea.

`bin` is **required** for the daemon to access the binary, and should be the name of the daemon's binary in the /daemons directory.

`conf_value` is optional but still a good idea to add—fill it if others will use this daemon.

`help` is purely optional—fill it in if you plan to distribute your daemon.

`name` is **mandatory**, it does not have to be the same as `bin` however, but it is the name the user will use with the interface.

`status` should be filled out as inactive when creating the cache.

## Uninstalling
If you would like to uninstall, simply download the uninstall.sh script, place it next to your LDM directory, and run the script. The script will delete the directory and remove LDM from PATH.

## Personal notes
I started building this project as my gateway into the world of low-level dev and C++. I have had an IMMENSE amount of fun and have learned A LOT about systems programming and C++, and I even learned some under-the-hood workings that languages such as Python or Java abstract away. I hope to come back to this project, creating some sort of marketplace, because right now I want to work on something else and learn something new. I am truly impressed at what I have been able to make, and this will hopefully be the start of something :)
