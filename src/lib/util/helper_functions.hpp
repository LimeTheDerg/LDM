#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "../json/json.hpp"

enum program_state {
    PARENT,
    CHILD,
    ERROR
};

inline std::string find_process_path() {
    char path[PATH_MAX];
    // The proc.self/exe symlink always points to the process that accesses it
    ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
    // I count is -1 something has gone catastrophically wrong
    if (count != -1) {
        path[count] = '\0';
        const std::string path_str = path;
        return path_str.substr(0, path_str.find_last_of('/'));
    }
    std::cerr << "Something has gone catastrophically wrong.";
    return "";
}

inline nlohmann::json read_cache() {
    std::ifstream cache_file;
    cache_file.open(find_process_path() + "/daemon_cache.json");
    if (!cache_file.is_open()) {
        std::cerr << "Failed to open cache file" << "\n";
    }
    nlohmann::json cache_json = nlohmann::json::parse(cache_file);
    return cache_json;
}

inline std::vector<std::string> split_by_space(std::string s) {
    std::vector<std::string> arg_vector;
    std::string arg;

    // Algorithm to search through string for whitespace and push the word to vector if found
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == ' ') {
            arg_vector.push_back(arg);
            arg.clear();
        }
        else arg += s[i];
    }
    arg_vector.push_back(arg);

    return arg_vector; // Return a pointer to the first element
}

inline program_state fork_execv_parent(const std::string& target, nlohmann::json &cache) {
    pid_t pid = 0;//fork(); // Create a child process

    if (pid < 0) {
        std::cerr << "Something has gone catastrophically wrong.";
        return ERROR;
    }

    // Child process has been found
    if (pid == 0) {
        std::string bin_name;
        int daemon_id;
        // Find the daemon in the cache and save its id for later
        for (int i = 0; i < cache.size(); i++) {
            if (cache["daemons"][i]["name"] == target) {
                bin_name = cache["daemons"][i]["bin"];
                daemon_id = i;
            }
        }

        // Find the bin's full path
        const std::string bin_path = find_process_path() + "/" + bin_name;
        // Receive the arguments from the cache
        std::vector<std::string> arg_v = split_by_space("1 2 3 4 5");

        for (int i = 0; i < arg_v.size(); i++) {
            std::cout << arg_v[i] << "\n";
        }

        return PARENT;
    }
    return ERROR;
}

#endif
