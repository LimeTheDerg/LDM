#ifndef DAEMON_API_H
#define DAEMON_API_H

/**
 * Variable that contains the name of the binary
 */
inline std::string bin_name;

/**
 * Simple algorithm to find the bin name using the current path.
 * It works by shaving off anything before the last '/' character.
 * This is to be used primarily by the API itself, but you may use it if needed.
 * @param path the absolute path of the bin
 * @return the name of the bin
 */
std::string get_bin_name_from_path(const std::string& path);

/**
 * Method for logging. Adds the contents of message to the log file located in the bin path.
 * Useful for error logging.
 * @param message The log message.
 */
void daemon_api_log(const std::string& message);

/**
 * Method for finding the absolute path of the bin directory where all binaries, caches, and FIFOs are located.
 * @returns A std::string which contains the binary path.
 */
std::string find_bin_path();

/**
 * Method for finding the absolute path of the main IPC FIFO.
 * @return A std::string which contains the FIFO path.
 */
std::string find_fifo_path();

/**
 * MUST BE PLACED AT TOP OF DAEMON SOURCE CODE \n
 * Method for turing the process image into a proper daemon.
 */
void daemonize(const char* bin);

/**
 * IPC method. \n
 * Sends a packet of info containing content to the main FIFO for the listener to read.
 * Note: writing more than 4096 bytes to the FIFO will result in jumbled results.
 * @param content The message to be sent to the main FIFO.
 */
void write_fifo(const std::string &content);

/**
 * IPC method. \n
 * MUST BE PLACED IN EVENT LOOP OF DAEMON SOURCE CODE.
 * Reads from the kill file located in the binary path. If the name of the process is found in the kill file, the daemon will exit.
 * This is what allows the daemon to stop properly.
 */
void read_kill_file();

/**
 * IPC method. \n
 * Reads the sendfile populated by the interface for information and checks if it meant for this daemon.
 * If meant for this daemon, it will return the contents of the message.
 * @return The contents of the message.
 */
std::string read_send_file();

#endif //DAEMON_API_H
