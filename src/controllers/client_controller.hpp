#pragma once
#include <crow.h>
#include "../models/client.hpp"

class ClientController {
private:
    static void add_cors_headers(crow::response& res) {
        res.add_header("Access-Control-Allow-Origin", "http://localhost:3000");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        res.add_header("Access-Control-Allow-Credentials", "true");
    }

public:
    static void RegisterRoutes(crow::SimpleApp& app) {
        CROW_ROUTE(app, "/clients")
            .methods("GET"_method, "OPTIONS"_method)
            ([](const crow::request& req) {
                if (req.method == "OPTIONS"_method) {
                    crow::response res(204);
                    add_cors_headers(res);
                    return res;
                }

                auto clients = get_all_clients();
                crow::json::wvalue response;
                std::vector<crow::json::wvalue> clientsJson;

                for (const auto& client : clients) {
                    crow::json::wvalue clientJson;
                    clientJson["client_id"] = client.client_id;
                    clientJson["client_name"] = client.client_name;
                    clientJson["phone"] = client.phone;
                    clientJson["email"] = client.email;
                    clientsJson.push_back(std::move(clientJson));
                }

                response["clients"] = std::move(clientsJson);
                auto res = crow::response(response);
                add_cors_headers(res);
                return res;
            });
    }
}; 