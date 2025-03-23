#pragma once
#include <mysql/mysql.h>
#include <iostream>

inline MYSQL* get_db() {
    MYSQL* conn = mysql_init(nullptr);
    if (!conn) {
        std::cerr << "MySQL initialization failed" << std::endl;
        return nullptr;
    }
    if (!mysql_real_connect(conn, "localhost", "root", "Z0ke3VB1", "calendaric", 0, nullptr, 0)) {
        std::cerr << "MySQL connection failed: " << mysql_error(conn) << std::endl;
        return nullptr;
    }
    std::cerr << "MySQL connection successful" << std::endl;
    return conn;
}
