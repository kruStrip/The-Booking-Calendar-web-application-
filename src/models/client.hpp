#pragma once

#include <string>
#include <vector>
#include <chrono>

struct Client {
    int client_id;
    std::string client_name;
    std::string phone;
    std::string email;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
    std::chrono::system_clock::time_point deleted_at;
};

std::vector<Client> get_all_clients(); 