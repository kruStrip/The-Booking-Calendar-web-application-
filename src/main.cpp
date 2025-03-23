#include <crow.h>
#include "controllers/booking_controller.hpp"

int main() {
    crow::SimpleApp app;
    setupBookingRoutes(app);
    app.port(8180).multithreaded().run();
}
