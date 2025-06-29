#ifndef DAEMON_FUNCTION_WRAPPER_HPP
#define DAEMON_FUNCTION_WRAPPER_HPP

#include <fcntl.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/stat.h>

#define BUF_MAX 512

inline std::string find_process_path() {
    char path[PATH_MAX];
    // The proc.self/exe symlink always points to the process that accesses it
    const ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
    // If count is -1 something has gone catastrophically wrong
    if (count != -1) {
        path[count] = '\0';
        const std::string path_str = path;
        return path_str.substr(0, path_str.find_last_of('/'));
    }
    exit(EXIT_FAILURE);
    return "";
}

inline std::string find_fifo_path() {
    std::string fifo_path = find_process_path() + "/fifo";
    return fifo_path;
}

inline void daemonize() {
    // Set permissions for other files, owner can do anything it wishes, but everything else has read and execute access only
    umask(022);
    // Start a new session
    const pid_t sid = setsid();

    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    // Change the working directory to process path
    chdir(find_process_path().c_str());
    // Daemons should not be able to access stdout
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

inline void write_fifo(const char* fifo_path, const std::string &content) {
    const int fifo_int = open(fifo_path, O_WRONLY | O_NONBLOCK);
    write(fifo_int, content.c_str(), BUF_MAX);
    close(fifo_int);
}

inline void read_kill_file() {

}

inline void log(std::string message) {
    
}

#endif
