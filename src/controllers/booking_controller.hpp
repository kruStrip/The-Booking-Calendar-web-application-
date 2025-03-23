#pragma once
#include <crow.h>
#include "../models/booking.hpp"
#include <chrono>
#include <sstream>
#include <iomanip>

void setupBookingRoutes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/bookings").methods(crow::HTTPMethod::Post)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json) return crow::response(400, "Invalid JSON");

        std::cerr << "Received JSON: " << req.body << std::endl;

        Booking b{
            .event_id = 0,  // Will be set by DB
            .description = json["description"].s(),
            .price = std::stod(json["price"].s()),
            .service_id = json.has("service_id") ? static_cast<int>(json["service_id"].i()) : 0,
            .client_id = json.has("client_id") ? static_cast<int>(json["client_id"].i()) : 0,
            .date = std::chrono::system_clock::now(),  // Will be set properly below
            .start_time = std::chrono::system_clock::now(),  // Will be set properly below
            .end_time = std::chrono::system_clock::now(),  // Will be set properly below
            .created_at = std::chrono::system_clock::now(),
            .updated_at = std::chrono::system_clock::now(),
            .created_by = json.has("created_by") ? static_cast<int>(json["created_by"].i()) : 0,
            .updated_by = json.has("updated_by") ? static_cast<int>(json["updated_by"].i()) : 0,
            .color = json["color"].s(),
            .amocrm_lead_id = json.has("amocrm_lead_id") ? static_cast<int>(json["amocrm_lead_id"].i()) : 0
        };

        std::cerr << "Parsed booking data: " << b.description << ", " << b.price << std::endl;

        std::tm tm{};
        std::istringstream(std::string(json["date"].s())) >> std::get_time(&tm, "%Y-%m-%d");
        b.date = std::chrono::system_clock::from_time_t(std::mktime(&tm));

        std::istringstream(std::string(json["date"].s()) + " " + std::string(json["start_time"].s())) >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        b.start_time = std::chrono::system_clock::from_time_t(std::mktime(&tm));

        std::istringstream(std::string(json["date"].s()) + " " + std::string(json["end_time"].s())) >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        b.end_time = std::chrono::system_clock::from_time_t(std::mktime(&tm));

        std::cerr << "Attempting to insert booking..." << std::endl;

        return insert_booking(b) ? crow::response(200, "Booking created") : crow::response(500, "Failed to insert booking");
    });

    CROW_ROUTE(app, "/bookings/<int>").methods(crow::HTTPMethod::Delete)([](int event_id) {
        if (delete_booking(event_id)) {
            return crow::response(200, "Booking deleted");
        }
        return crow::response(404, "Booking not found");
    });

    CROW_ROUTE(app, "/bookings/search/<string>").methods(crow::HTTPMethod::Get)([](const crow::request& req, const std::string& service_name) {
        auto results = search_booking_by_service_name(service_name);
        if (results.empty()) {
            return crow::response(404, "No bookings found");
        }

        crow::json::wvalue x;
        x["bookings"] = crow::json::wvalue::list();
        for (const auto& booking : results) {
            x["bookings"][x["bookings"].size()] = crow::json::wvalue{{"event_id", booking.event_id}, {"description", booking.description}};
        }
        return crow::response(x);
    });
}
