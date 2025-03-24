#include "service.hpp"
#include "../db/connection.hpp"
#include <mysql/mysql.h>
#include <iostream>

std::vector<Service> get_all_services() {
    std::vector<Service> services;
    MYSQL* conn = get_db();
    
    if (!conn) {
        std::cerr << "Failed to connect to database" << std::endl;
        return services;
    }

    const char* query = "SELECT service_id, service_name, work_start_time, work_end_time FROM services WHERE deleted_at IS NULL";
    
    if (mysql_query(conn, query)) {
        std::cerr << "Failed to execute query: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return services;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        std::cerr << "Failed to get result set: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return services;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        Service service;
        service.service_id = row[0] ? std::stoi(row[0]) : 0;
        service.service_name = row[1] ? row[1] : "";
        service.work_start_time = row[2] ? row[2] : "";
        service.work_end_time = row[3] ? row[3] : "";
        services.push_back(service);
    }

    mysql_free_result(result);
    mysql_close(conn);
    return services;
} 