#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H
#endif

#include "../json/json.hpp"

inline std::string find_process_path() {
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (count != -1) {
        path[count] = '\0';
        const std::string path_str = path;
        return path_str.substr(0, path_str.find_last_of('/'));
    }
    std::cerr << "Something has gone catastrophically wrong.";
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