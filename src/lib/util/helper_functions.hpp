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
    const ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
    // If count is -1 something has gone catastrophically wrong
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

inline std::vector<std::string> split_by_space(const std::string &s) {
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

    return arg_vector;
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
        int daemon_id = 0;
        // Find the daemon in the cache and save its id for later
        for (int i = 0; i < cache.size(); i++) {
            if (cache["daemons"][i]["name"] == target) {
                bin_name = cache["daemons"][i]["bin"];
                daemon_id = i;
            }
        }

        // Find the bin's full path
        const std::string bin_path = find_process_path() + "/daemons/" + bin_name;
        // Receive the arguments from the cache
        std::vector<std::string> arg_strs = split_by_space(cache["daemons"][daemon_id]["args"]);

        // Given the std::string vector, copy the contents into a new vector of c-style strings
        std::vector<char*> arg_ptrs;
        for (std::string& str : arg_strs) {
            arg_ptrs.push_back(&str[0]);
        }

        arg_ptrs.push_back(nullptr); // execv expects a null-terminated array
        std::cout << *(arg_ptrs.data());

        execv(bin_path.c_str(), arg_ptrs.data());

        return CHILD; // Will not be run, but if I remove it CMake will yell at me
    }
    return PARENT;
}

#endif
