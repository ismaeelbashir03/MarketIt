#include "crow.h" // all crow headers
#include "EventManager.cpp"
#include "UserManager.cpp"
#include <iostream>

class Server {
private:
    crow::SimpleApp app;
    EventManager market;
    UserManager users;

public:
    Server() {
        /*
         * GET /api/events
         * gets all events from the market 
         */
        CROW_ROUTE(app, "/api/events")
            .methods("GET"_method, "POST"_method)
            ([this](const crow::request& req) {
                if (req.method == "GET"_method) {
                    return crow::response(market.getEvents());
                } else {
                    market.createEvent(req.body);
                    return crow::response(201, "Event created");
                }
            });
        
        CROW_ROUTE(app, "/api/user")
            .methods("POST"_method, "GET"_method)
            ([this](const crow::request& req) {
                json j = json::parse(req.body);
                if (req.method == "GET"_method) {
                    return crow::response(users.getUser(j["id"]).getJson().dump());
                } else {
                    User u(j["id"], j["name"]);
                    users.createUser(u);
                    return crow::response(201, "User created");
                }
            });
        
        CROW_ROUTE(app, "/api/order")
            .methods("POST"_method)
            ([this](const crow::request& req) {
                json j = json::parse(req.body);
                int user_id = j["user_id"];
                int response = market.addOrder(req.body);
                if (response == -1) {
                    return crow::response(201, "Order not created");
                }
                users.addOrder(user_id, req.body);
                return crow::response(201, "Order created");
            });
    }

    void run(int port) {
        app.port(port).multithreaded().run();
    }
};