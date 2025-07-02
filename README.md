# LDM
**LDM** is a lightweight daemon control and communication system written in C++. It lets you start, stop, and send messages to custom daemons using a unified CLI, with IPC handled via FIFOs or text-based signaling.

## Features

- Spawn and kill daemons at will with a CLI application
- Read messages sent by any active daemon through one listener CLI application
- Configure daemons to change behaviour
- Send information to daemons that support sending
- No external libraries/dependencies (However, it does use the Nlhomann JSON single include header file.)
- Very Lightweight, built with Arch Linux but will likely work on most distros

## Structure
LDM is split into two applications, one interface, and one listener. The interface is used to spawn, kill, configure, or
send messages to daemons, while the listener listens to messages sent by any active daemons. For IPC, the project uses Linux FIFOs
or simply text files to communicate.

The interface has a help command that goes over how to use the application in detail (including listener), just run this command:
```
ldmi help
```

## Installing
You have two options:
1. Use the installation script: **install.sh**, which will configure PATH for you.
2. Download the bin directory as a whole, and choose to configure PATH yourself or not.

Note: Going with option 2 and not setting PATH means you have to move to the directory of the binaries within the shell
and run commands with a ./ at the start, for example:
```
./ldmi help
```

Both options are available in the release found here: <insert-release-link-here-later>
