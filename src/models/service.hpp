#pragma once

#include <string>
#include <vector>
#include <chrono>

struct Service {
    int service_id;
    std::string service_name;
    std::string work_start_time;
    std::string work_end_time;
};

std::vector<Service> get_all_services(); 