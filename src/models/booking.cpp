#include "booking.hpp"
#include "../db/connection.hpp"
#include <iomanip>
#include <sstream>
#include <iostream>
#include <mysql/mysql.h>
#include <cstring>
#include <string>

std::string urlDecode(const std::string &value) {
    std::ostringstream decoded;
    for (size_t i = 0; i < value.length(); ++i) {
        if (value[i] == '%') {
            int hexValue;
            std::istringstream(value.substr(i + 1, 2)) >> std::hex >> hexValue;
            decoded << static_cast<char>(hexValue);
            i += 2;
        } else if (value[i] == '+') {
            decoded << ' ';
        } else {
            decoded << value[i];
        }
    }
    return decoded.str();
}

std::string timepoint_to_string(const std::chrono::system_clock::time_point& tp, const std::string& format = "%Y-%m-%d %H:%M:%S") {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, format.c_str());
    return oss.str();
}

bool insert_booking(const Booking& booking) {
    std::cerr << "Attempting to connect to the database..." << std::endl;
    MYSQL* conn = get_db();
    if (!conn) {
        std::cerr << "Failed to connect to database" << std::endl;
        return false;
    }
    std::cerr << "Connected to the database successfully." << std::endl;

    std::stringstream query;
    query << "INSERT INTO bookings (description, price, service_id, client_id, "
          << "date, start_time, end_time, created_at, updated_at, "
          << "created_by, updated_by, color, amocrm_lead_id) VALUES "
          << "(?, ?, ?, ?, ?, ?, ?, NOW(), NOW(), ?, ?, ?, ?)";

    std::cerr << "Preparing query: " << query.str() << std::endl;

    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    if (!stmt) {
        std::cerr << "mysql_stmt_init() failed: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }

    if (mysql_stmt_prepare(stmt, query.str().c_str(), query.str().length())) {
        std::cerr << "mysql_stmt_prepare() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return false;
    }

    MYSQL_BIND bind[13];
    memset(bind, 0, sizeof(bind));

    // Convert time points to MySQL DATETIME format
    auto to_mysql_time = [](const std::chrono::system_clock::time_point& tp) {
        MYSQL_TIME mt{};
        time_t tt = std::chrono::system_clock::to_time_t(tp);
        tm* ptm = localtime(&tt);
        mt.year = ptm->tm_year + 1900;
        mt.month = ptm->tm_mon + 1;
        mt.day = ptm->tm_mday;
        mt.hour = ptm->tm_hour;
        mt.minute = ptm->tm_min;
        mt.second = ptm->tm_sec;
        return mt;
    };

    // Bind parameters
    unsigned long str_length = booking.description.length();
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void*)booking.description.c_str();
    bind[0].buffer_length = str_length;
    bind[0].length = &str_length;

    bind[1].buffer_type = MYSQL_TYPE_DOUBLE;
    bind[1].buffer = (void*)&booking.price;

    bind[2].buffer_type = booking.service_id ? MYSQL_TYPE_LONG : MYSQL_TYPE_NULL;
    bind[2].buffer = booking.service_id ? (void*)&booking.service_id : nullptr;

    bind[3].buffer_type = booking.client_id ? MYSQL_TYPE_LONG : MYSQL_TYPE_NULL;
    bind[3].buffer = booking.client_id ? (void*)&booking.client_id : nullptr;

    MYSQL_TIME date_mt = to_mysql_time(booking.date);
    bind[4].buffer_type = MYSQL_TYPE_DATE;
    bind[4].buffer = (void*)&date_mt;

    MYSQL_TIME start_mt = to_mysql_time(booking.start_time);
    bind[5].buffer_type = MYSQL_TYPE_DATETIME;
    bind[5].buffer = (void*)&start_mt;

    MYSQL_TIME end_mt = to_mysql_time(booking.end_time);
    bind[6].buffer_type = MYSQL_TYPE_DATETIME;
    bind[6].buffer = (void*)&end_mt;

    bind[7].buffer_type = booking.created_by ? MYSQL_TYPE_LONG : MYSQL_TYPE_NULL;
    bind[7].buffer = booking.created_by ? (void*)&booking.created_by : nullptr;

    bind[8].buffer_type = booking.updated_by ? MYSQL_TYPE_LONG : MYSQL_TYPE_NULL;
    bind[8].buffer = booking.updated_by ? (void*)&booking.updated_by : nullptr;

    unsigned long color_length = booking.color.length();
    bind[9].buffer_type = MYSQL_TYPE_STRING;
    bind[9].buffer = (void*)booking.color.c_str();
    bind[9].buffer_length = color_length;
    bind[9].length = &color_length;

    bind[10].buffer_type = booking.amocrm_lead_id ? MYSQL_TYPE_LONG : MYSQL_TYPE_NULL;
    bind[10].buffer = booking.amocrm_lead_id ? (void*)&booking.amocrm_lead_id : nullptr;

    std::cerr << "Binding parameters..." << std::endl;
    std::cerr << "Description: " << booking.description << std::endl;
    std::cerr << "Price: " << booking.price << std::endl;
    std::cerr << "Service ID: " << booking.service_id << std::endl;
    std::cerr << "Client ID: " << booking.client_id << std::endl;
    std::cerr << "Date: " << timepoint_to_string(booking.date) << std::endl;
    std::cerr << "Start Time: " << timepoint_to_string(booking.start_time) << std::endl;
    std::cerr << "End Time: " << timepoint_to_string(booking.end_time) << std::endl;
    std::cerr << "Created By: " << booking.created_by << std::endl;
    std::cerr << "Updated By: " << booking.updated_by << std::endl;
    std::cerr << "Color: " << booking.color << std::endl;
    std::cerr << "AmoCRM Lead ID: " << booking.amocrm_lead_id << std::endl;

    if (mysql_stmt_bind_param(stmt, bind)) {
        std::cerr << "mysql_stmt_bind_param() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return false;
    }

    bool success = !mysql_stmt_execute(stmt);
    if (!success) {
        std::cerr << "mysql_stmt_execute() failed: " << mysql_stmt_error(stmt) << std::endl;
    }

    mysql_stmt_close(stmt);
    mysql_close(conn);
    return success;
}

bool delete_booking(int event_id) {
    std::cerr << "Attempting to connect to the database..." << std::endl;
    MYSQL* conn = get_db();
    if (!conn) {
        std::cerr << "Failed to connect to database" << std::endl;
        return false;
    }
    std::cerr << "Connected to the database successfully." << std::endl;

    std::stringstream query;
    query << "DELETE FROM bookings WHERE event_id = " << event_id;
    if (mysql_query(conn, query.str().c_str())) {
        std::cerr << "Failed to delete booking: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }

    if (mysql_affected_rows(conn) > 0) {
        std::cerr << "Booking deleted successfully." << std::endl;
        mysql_close(conn);
        return true;
    }
    std::cerr << "Booking not found." << std::endl;
    mysql_close(conn);
    return false;
}

/*std::vector<Booking> search_booking_by_description(const std::string& description) {
    std::vector<Booking> results;
    std::cerr << "Attempting to connect to the database..." << std::endl;
    MYSQL* conn = get_db();
    if (!conn) {
        std::cerr << "Failed to connect to database" << std::endl;
        return results;
    }
    std::cerr << "Connected to the database successfully." << std::endl;

    std::stringstream query;
    query << "SELECT * FROM bookings WHERE description LIKE '%" << description << "%'";
    if (mysql_query(conn, query.str().c_str())) {
        std::cerr << "Failed to search bookings: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return results;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res) {
        std::cerr << "Failed to store result: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return results;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        Booking b;
        b.event_id = std::stoi(row[0]);
        b.description = row[1];
        b.price = std::stod(row[2]);
        b.service_id = std::stoi(row[3]);
        b.client_id = std::stoi(row[4]);
        // Parse other fields as needed
        results.push_back(b);
    }

    mysql_free_result(res);
    mysql_close(conn);
    return results;
}*/

std::vector<Booking> search_booking_by_service_name(const std::string& service_name) {
    std::vector<Booking> results;
    std::cerr << "Attempting to connect to the database..." << std::endl;
    MYSQL* conn = get_db();
    if (!conn) {
        std::cerr << "Failed to connect to database" << std::endl;
        return results;
    }
    std::cerr << "Connected to the database successfully." << std::endl;

    std::string decodedServiceName = urlDecode(service_name);
    std::stringstream query;
    query << "SELECT b.* FROM bookings b "
          << "JOIN services s ON b.service_id = s.service_id "
          << "WHERE s.service_name LIKE '%" << decodedServiceName << "%'";
    std::cerr << "Executing query: " << query.str() << std::endl;

    if (mysql_query(conn, query.str().c_str())) {
        std::cerr << "Failed to search bookings: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return results;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res) {
        std::cerr << "Failed to store result: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return results;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        Booking b;
        b.event_id = row[0] ? std::stoi(row[0]) : 0;
        b.description = row[1] ? row[1] : "";
        b.price = row[2] ? std::stod(row[2]) : 0.0;
        b.service_id = row[3] ? std::stoi(row[3]) : 0;
        b.client_id = row[4] ? std::stoi(row[4]) : 0;
        // Parse other fields as needed
        std::cerr << "Found booking: " << b.event_id << ", " << b.description << std::endl;
        results.push_back(b);
    }

    mysql_free_result(res);
    mysql_close(conn);
    std::cerr << "Search completed. Total bookings found: " << results.size() << std::endl;
    return results;
}