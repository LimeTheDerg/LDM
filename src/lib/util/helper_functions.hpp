#ifndef HELPER_FUNCTIONS_HPP
#define HELPER_FUNCTIONS_HPP

#include "../json/json.hpp"
#include <fstream>
#include <fcntl.h>

enum program_state {
    PARENT,
    CHILD,
    ERROR
};

inline std::string find_bin_path() {
    char path[PATH_MAX];
    // The proc.self/exe symlink always points to the process that accesses it
    const ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
    // If count is -1 something has gone catastrophically wrong
    if (count != -1) {
        path[count] = '\0';
        const std::string path_str = path;
        return path_str.substr(0, path_str.find_last_of('/'));
    }
    std::cerr << "Something has gone catastrophically wrong.";
    exit(EXIT_FAILURE);
    return "";
}

inline nlohmann::json read_cache() {
    std::ifstream cache_file;
    cache_file.open(find_bin_path() + "/daemon_cache.json");
    if (!cache_file.is_open()) {
        std::cerr << "Failed to open cache file" << "\n";
        exit(EXIT_FAILURE);
    }
    nlohmann::json cache_json = nlohmann::json::parse(cache_file);
    cache_file.close();
    return cache_json;
}

inline std::vector<std::string> split_by_space(const std::string &s, const std::string &bin) {
    std::vector<std::string> arg_vector;
    std::string arg;
    // Algorithm to search through string for whitespace and push the word to vector if found
    arg_vector.push_back(bin);
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == ' ') {
            arg_vector.push_back(arg);
            arg.clear();
        }
        else arg += s[i];
    }
    arg_vector.push_back(arg);

    return arg_vector;
}

inline program_state fork_execv_parent(const std::string& target, nlohmann::json &cache) {
    pid_t pid = fork(); // Create a child process

    if (pid < 0) {
        std::cerr << "Something has gone catastrophically wrong.";
        return ERROR;
    }

    // Child process has been found
    if (pid == 0) {
        std::string bin_name;
        int daemon_id = 0;

        // Find the daemon in the cache and save its id for later
        for (int i = 0; i < cache.size(); i++) {
            if (cache["daemons"][i]["name"] == target) {
                bin_name = cache["daemons"][i]["bin"];
                daemon_id = i;
            }
        }

        // Find the bin's full path
        const std::string bin_path = find_bin_path() + "/daemons/" + bin_name;
        // Receive the arguments from the cache
        std::vector<std::string> arg_strs = split_by_space(cache["daemons"][daemon_id]["args"], cache["daemons"][daemon_id]["bin"]);
        // Given the std::string vector, copy the contents into a new vector of c-style strings
        std::vector<char*> arg_ptrs;
        for (std::string& str : arg_strs) {
            arg_ptrs.push_back(&str[0]);
        }

        arg_ptrs.push_back(nullptr); // execv expects a null-terminated array
        execv(bin_path.c_str(), arg_ptrs.data());

        return CHILD; // Will not actually return if all goes smoothly, but if I remove it CMake will yell at me
    }
    return PARENT; // This is what tells the interface it should kill itself
}

inline void write_cache(nlohmann::json &cache) {
    std::ofstream cache_file;
    cache_file.open(find_bin_path() + "/daemon_cache.json", std::ios::trunc);

    if (!cache_file.is_open()) {
        std::cerr << "Failed to write to cache file" << "\n";
        exit(EXIT_FAILURE);
    }

    const std::string cache_json = cache.dump(4); // Creates an indented string using current cache in program
    cache_file << cache_json; // Write to cache
    cache_file.close();
}

inline void listener_clean_exit(int sig) {
    std::cout << "\n" << "Quitting..." << "\n";
    const std::string path = find_bin_path()+"/fifo";
    unlink(path.c_str());
    const int fifo_fd = open(path.c_str(), O_RDONLY);
    close(fifo_fd);
    exit(0);
}

inline void clear_kill_file() {
    std::ofstream kill;
    kill.open("kill", std::ios::trunc);
    const std::string garbage;
    kill << garbage;
    kill.close();
}

inline void write_kill_file(const std::string& daemon) {
    std::ofstream kill;
    kill.open("kill", std::ios::trunc);
    kill << daemon;
    kill.close();
}
#endif
