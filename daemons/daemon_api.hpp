#ifndef DAEMON_API
#define DAEMON_API

#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/stat.h>

inline std::string bin_name;

/**
 * Method for logging. Adds the contents of message to the log file located in the bin path.
 * Useful for error logging.
 * @param message The log message.
 */
inline void daemon_api_log(const std::string& message) {
    std::ofstream log;
    log.open("log", std::ios::app);
    log << message << "\n";
    log.close();
}

/**
 * Method for finding the absolute path of the bin directory where all binaries, caches, and FIFOs are located.
 * @returns A std::string which contains the binary path.
 */
inline std::string find_bin_path() {
    char path[PATH_MAX];
    // The proc.self/exe symlink always points to the process that accesses it
    const ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
    // If count is -1 something has gone catastrophically wrong
    if (count != -1) {
        path[count] = '\0';
        const std::string path_str = path;
        return path_str.substr(0, path_str.find_last_of('/')-8); // this will get the second last of '/' since /daemons is exactly 8 characters
    }
    exit(EXIT_FAILURE);
    return "";
}

/**
 * Method for finding the absolute path of the main IPC FIFO.
 * @return A std::string which contains the FIFO path.
 */
inline std::string find_fifo_path() {
    std::string fifo_path = find_bin_path() + "/fifo";
    return fifo_path;
}

/**
 * MUST BE PLACED AT TOP OF DAEMON SOURCE CODE \n
 * Method for turing the process image into a proper daemon.
 */
inline void daemonize(const char* bin) {
    bin_name = bin;

    umask(0);
    // Start a new session
    const pid_t sid = setsid();

    if (sid < 0) {
        std::stringstream error;
        error << "[ERROR] - FAILED TO DAEMONIZE: " << bin;
        daemon_api_log(error.str());
        exit(EXIT_FAILURE);
    }

    // Change the working directory to bin path
    chdir(find_bin_path().c_str());
    // Daemons should not be able to access stdout
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

/**
 * IPC method. \n
 * Sends a packet of info containing content to the main FIFO for the listener to read.
 * Note: writing more than 4096 bytes to the FIFO will result in jumbled results.
 * @param content The message to be sent to the main FIFO.
 */
inline void write_fifo(const std::string &content) {
    const int fifo_int = open(find_fifo_path().c_str(), O_WRONLY);
    write(fifo_int, content.c_str(), content.size());
    close(fifo_int);
}

/**
 * IPC method. \n
 * MUST BE PLACED IN EVENT LOOP OF DAEMON SOURCE CODE.
 * Reads from the kill file located in the binary path. If the name of the process is found in the kill file, the daemon will exit.
 * This is what allows the daemon to stop properly.
 */
inline void read_kill_file() {
    std::ifstream kill_file;
    kill_file.open("kill", std::ios::in);
    if (!kill_file.is_open()) {
        daemon_api_log("[ERROR] - FAILED TO READ KILL FILE: " + bin_name);
    }
    std::stringstream name;
    name << kill_file.rdbuf();
    if (name.str() == bin_name) {
        exit(0);
    }
    kill_file.close();
}

#endif
