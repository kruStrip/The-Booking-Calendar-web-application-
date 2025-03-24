#pragma once

#include <crow.h>
#include "../models/service.hpp"

class ServiceController {
private:
    static void add_cors_headers(crow::response& res) {
        res.add_header("Access-Control-Allow-Origin", "http://localhost:3000");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        res.add_header("Access-Control-Allow-Credentials", "true");
    }

public:
    static void RegisterRoutes(crow::SimpleApp& app) {
        CROW_ROUTE(app, "/services")
            .methods("GET"_method, "OPTIONS"_method)
            ([](const crow::request& req) {
                if (req.method == "OPTIONS"_method) {
                    crow::response res(204);
                    add_cors_headers(res);
                    return res;
                }

                auto services = get_all_services();
                crow::json::wvalue response;
                std::vector<crow::json::wvalue> servicesJson;

                for (const auto& service : services) {
                    crow::json::wvalue serviceJson;
                    serviceJson["service_id"] = service.service_id;
                    serviceJson["service_name"] = service.service_name;
                    serviceJson["work_start_time"] = service.work_start_time;
                    serviceJson["work_end_time"] = service.work_end_time;
                    servicesJson.push_back(std::move(serviceJson));
                }

                response["services"] = std::move(servicesJson);
                auto res = crow::response(response);
                add_cors_headers(res);
                return res;
            });
    }
}; 