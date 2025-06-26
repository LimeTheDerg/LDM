#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H
#endif

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

inline char split_args_by_space(std::string s) {
    std::vector<char*> arg_vector;
    const char* s_c = s.c_str();

    for (const char* p = s_c; *p != '\0'; ++p) {

    }

}

inline program_state fork_execv_parent(const std::string& target, nlohmann::json &cache) {
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Something has gone catastrophically wrong.";
        return ERROR;
    }
    // Child process has been found
    if (pid == 0) {
        std::string bin_name = "";
        for (int i = 0; i < cache.size(); i++) {
            if (target == cache["daemons"][i]["name"]) {
                bin_name = cache["daemons"][i]["bin"];
            }
        }
        const std::string bin_path = find_process_path() + "/" + bin_name;
        const std::string fifty = "50";
        execv(bin_path.c_str(), );
    }
}