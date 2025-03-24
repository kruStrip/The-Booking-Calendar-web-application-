#pragma once
#include <crow.h>
#include "../models/booking.hpp"
#include <chrono>
#include <sstream>
#include <iomanip>

class BookingController {
private:
    static void add_cors_headers(crow::response& res) {
        res.add_header("Access-Control-Allow-Origin", "http://localhost:3000");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        res.add_header("Access-Control-Allow-Credentials", "true");
    }

public:
    static void RegisterRoutes(crow::SimpleApp& app) {
        CROW_ROUTE(app, "/bookings")
            .methods("POST"_method, "OPTIONS"_method)
            ([](const crow::request& req) {
                if (req.method == "OPTIONS"_method) {
                    crow::response res(204);
                    add_cors_headers(res);
                    return res;
                }

                std::cout << "Received booking request with body: " << req.body << std::endl;

                auto x = crow::json::load(req.body);
                if (!x) {
                    auto res = crow::response(400, "Invalid JSON");
                    add_cors_headers(res);
                    return res;
                }

                Booking booking;
                try {
                    booking.description = x["description"].s();
                    booking.price = std::stod(x["price"].s());
                    booking.service_id = std::stoi(x["service_id"].s());
                    if (x.has("client_id") && x["client_id"].s().size() > 0) {
                        booking.client_id = std::stoi(x["client_id"].s());
                    }
                    
                    // Parse date and times
                    std::string date = x["date"].s();
                    std::string start_time = x["start_time"].s();
                    std::string end_time = x["end_time"].s();

                    std::cout << "Parsed data: date=" << date << ", start_time=" << start_time << ", end_time=" << end_time << std::endl;
                    
                    // Convert to time_point
                    std::tm tm = {};
                    std::istringstream date_ss(date);
                    date_ss >> std::get_time(&tm, "%Y-%m-%d");
                    booking.date = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                    
                    std::istringstream start_ss(start_time);
                    start_ss >> std::get_time(&tm, "%H:%M:%S");
                    booking.start_time = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                    
                    std::istringstream end_ss(end_time);
                    end_ss >> std::get_time(&tm, "%H:%M:%S");
                    booking.end_time = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                    
                    booking.color = x["color"].s();

                    std::cout << "Successfully parsed all booking data" << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "Error parsing booking data: " << e.what() << std::endl;
                    auto res = crow::response(400, std::string("Invalid data: ") + e.what());
                    add_cors_headers(res);
                    return res;
                }

                if (insert_booking(booking)) {
                    std::cout << "Successfully inserted booking" << std::endl;
                    auto res = crow::response(201);
                    add_cors_headers(res);
                    return res;
                } else {
                    std::cout << "Failed to insert booking" << std::endl;
                    auto res = crow::response(500, "Failed to create booking");
                    add_cors_headers(res);
                    return res;
                }
            });

        CROW_ROUTE(app, "/bookings/date/<string>")
            .methods("GET"_method, "OPTIONS"_method)
            ([](const crow::request& req, std::string date) {
                if (req.method == "OPTIONS"_method) {
                    auto res = crow::response(204);
                    add_cors_headers(res);
                    return res;
                }

                auto bookings = get_bookings_by_date(date);
                crow::json::wvalue response;
                std::vector<crow::json::wvalue> bookingsJson;

                for (const auto& booking : bookings) {
                    crow::json::wvalue bookingJson;
                    bookingJson["event_id"] = booking.event_id;
                    bookingJson["description"] = booking.description;
                    bookingJson["price"] = booking.price;
                    bookingJson["service_id"] = booking.service_id;
                    bookingJson["client_id"] = booking.client_id;
                    bookingJson["date"] = timepoint_to_string(booking.date, "%Y-%m-%d");
                    bookingJson["start_time"] = timepoint_to_string(booking.start_time, "%H:%M:%S");
                    bookingJson["end_time"] = timepoint_to_string(booking.end_time, "%H:%M:%S");
                    bookingJson["color"] = booking.color;
                    bookingsJson.push_back(std::move(bookingJson));
                }

                response["bookings"] = std::move(bookingsJson);
                auto res = crow::response(response);
                add_cors_headers(res);
                return res;
            });

        CROW_ROUTE(app, "/bookings/<int>")
            .methods("DELETE"_method, "OPTIONS"_method)
            ([](const crow::request& req, int event_id) {
                if (req.method == "OPTIONS"_method) {
                    auto res = crow::response(204);
                    add_cors_headers(res);
                    return res;
                }

                if (delete_booking(event_id)) {
                    auto res = crow::response(200, "Booking deleted");
                    add_cors_headers(res);
                    return res;
                }
                auto res = crow::response(404, "Booking not found");
                add_cors_headers(res);
                return res;
            });

        CROW_ROUTE(app, "/bookings/search/<string>")
            .methods("GET"_method, "OPTIONS"_method)
            ([](const crow::request& req, const std::string& service_name) {
                if (req.method == "OPTIONS"_method) {
                    auto res = crow::response(204);
                    add_cors_headers(res);
                    return res;
                }

                auto results = search_booking_by_service_name(service_name);
                if (results.empty()) {
                    auto res = crow::response(404, "No bookings found");
                    add_cors_headers(res);
                    return res;
                }

                crow::json::wvalue x;
                x["bookings"] = crow::json::wvalue::list();
                for (const auto& booking : results) {
                    x["bookings"][x["bookings"].size()] = crow::json::wvalue{{"event_id", booking.event_id}, {"description", booking.description}};
                }
                auto res = crow::response(x);
                add_cors_headers(res);
                return res;
            });
    }
};
