#pragma once
#include <crow.h>

class ClientController {
public:
    static void RegisterRoutes(crow::SimpleApp& app) {
        CROW_ROUTE(app, "/clients")
            .methods("GET"_method, "OPTIONS"_method)
            ([](const crow::request& req) {
                if (req.method == "OPTIONS"_method) {
                    auto res = crow::response(204);
                    res.add_header("Access-Control-Allow-Origin", "http://localhost:3000");
                    res.add_header("Access-Control-Allow-Methods", "GET, OPTIONS");
                    res.add_header("Access-Control-Allow-Headers", "*");
                    return res;
                }

                // Пока возвращаем пустой список клиентов
                crow::json::wvalue response;
                response["clients"] = crow::json::wvalue::list();
                
                auto res = crow::response(response);
                res.add_header("Access-Control-Allow-Origin", "http://localhost:3000");
                return res;
            });
    }
}; 