#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H
#include "../json/json.hpp"

enum program_state {
    PARENT,
    CHILD,
    ERROR
};

std::string find_bin_path();
nlohmann::json read_cache();
std::vector<std::string> split_by_space(const std::string &s, const std::string &bin);
program_state fork_execv_parent(const std::string& target, nlohmann::json &cache);
void write_cache(nlohmann::json &cache);
void listener_clean_exit(int sig);
void write_kill_file(const std::string& daemon);


#endif //HELPER_FUNCTIONS_H
