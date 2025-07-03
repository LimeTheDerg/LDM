# LDM
**LDM** is a lightweight daemon control and communication system written in C++. It lets you start, stop, and send messages to custom daemons using a unified CLI, with IPC handled via FIFOs or text-based signaling.

## Features

- Spawn and kill daemons at will with a CLI application
- Read messages sent by any active daemon through one listener CLI application
- Configure daemons to change behaviour
- Send information to daemons that support sending
- No external libraries/dependencies (However, it does use the Nlhomann JSON single include header file.)
- Very Lightweight, built with Arch Linux but will likely work on most distros
- Base installation comes with 2 daemons, vault and cpu-temperature-monitor
- A very easy to use, lightweight API to build your own daemons with ease

## Structure
LDM is split into two applications, one interface, and one listener. The interface is used to spawn, kill, configure, or
send messages to daemons, while the listener listens to messages sent by any active daemons. For IPC, the project uses Linux FIFOs
or simply text files to communicate.

The interface has a help command that goes over how to use the application in detail (including listener), just run this command:
````
ldmi help
````

## Installing
You have two options:
1. Use the installation script: **install.sh**, which will configure PATH for you.
2. Download the bin directory as a whole, and choose to configure PATH yourself or not.

Note: Going with option 2 and not setting PATH means you have to move to the directory of the binaries within the shell
and run commands with a ./ at the start, for example:
```
./ldmi help
```

Both options are available in the release found here: https://github.com/LimeTheDerg/LDM/releases/tag/First-Release

## Example usage
### Here is an example interation with LDM, they are running at the same time:
Interacting with daemons through ldmi:
````
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
````
Listening to daemons through ldml:
````
ldml
>>> Opening daemon listener...
>>> Press Ctrl+C to quit
>>> vault - [INFO] - The vault's current contents are: empty
>>>
````
# Daemon API documentation
To make it as easy as possible to make your own daemons, I have written a small API compatible with LDM.
It contains some functions to simplify the process of creating a daemon significantly.

You can figure the functionality by reading the comments in this file, which is the entire API.
To use the API, all you have to do is include it.
All the functions are commented and very self-explanatory.

https://github.com/LimeTheDerg/LDM/blob/main/daemons/daemon_api.hpp

You will need to create a cache entry yourself by modifying the daemon_cache.json file, read on for more details.


### Here is a quick tutorial on what you should do: