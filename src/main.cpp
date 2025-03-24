#include <crow.h>
#include "controllers/booking_controller.hpp"
#include "controllers/service_controller.hpp"
#include "controllers/client_controller.hpp"

int main() {
    crow::SimpleApp app;
    
    // Register routes
    BookingController::RegisterRoutes(app);
    ServiceController::RegisterRoutes(app);
    ClientController::RegisterRoutes(app);
    
    app.port(8180).multithreaded().run();
    return 0;
}
