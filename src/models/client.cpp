#include "client.hpp"
#include "../db/connection.hpp"
#include <mysql/mysql.h>
#include <iostream>

std::vector<Client> get_all_clients() {
    std::vector<Client> clients;
    MYSQL* conn = get_db();
    
    if (!conn) {
        std::cerr << "Failed to connect to database" << std::endl;
        return clients;
    }

    const char* query = "SELECT client_id, client_name, phone FROM clients WHERE deleted_at IS NULL";
    std::cerr << "Executing query: " << query << std::endl;
    
    if (mysql_query(conn, query)) {
        std::cerr << "Failed to execute query: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return clients;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        std::cerr << "Failed to get result set: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return clients;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        std::cerr << "Found client: " << (row[0] ? row[0] : "null") << ", " 
                  << (row[1] ? row[1] : "null") << ", "
                  << (row[2] ? row[2] : "null") << std::endl;
                  
        Client client;
        client.client_id = row[0] ? std::stoi(row[0]) : 0;
        client.client_name = row[1] ? row[1] : "";
        client.phone = row[2] ? row[2] : "";
        clients.push_back(client);
    }

    std::cerr << "Total clients found: " << clients.size() << std::endl;

    mysql_free_result(result);
    mysql_close(conn);
    return clients;
} 