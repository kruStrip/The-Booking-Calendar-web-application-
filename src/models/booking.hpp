#pragma once

#include <string>
#include <optional>
#include <chrono>
#include <vector>

struct Booking {
    int event_id;
    std::string description;
    double price;

    int service_id;
    int client_id;

    std::chrono::system_clock::time_point date;
    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point end_time;

    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;

    int created_by;
    int updated_by;

    std::string color;
    int amocrm_lead_id;

    std::optional<std::chrono::system_clock::time_point> deleted_at;
};

bool insert_booking(const Booking& booking);
bool delete_booking(int event_id);
std::vector<Booking> get_bookings_by_date(const std::string& date);
//std::vector<Booking> search_booking_by_description(const std::string& description);
std::vector<Booking> search_booking_by_service_name(const std::string& service_name);

std::string timepoint_to_string(const std::chrono::system_clock::time_point& tp, const std::string& format = "%Y-%m-%d %H:%M:%S");
