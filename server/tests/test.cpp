#include <iostream>
#include <thread>
#include "httplib.h" // Include cpp-httplib for HTTP requests
#include "../src/Server.cpp"

void testServer() {
    // Start server in a separate thread
    Server server;
    std::thread serverThread([&server]() { server.run(18080); });

    // Allow server to start
    std::this_thread::sleep_for(std::chrono::seconds(1));

    try {
        httplib::Client client("0.0.0.0", 18080);

        // Test GET /api/events
        {
            auto response = client.Get("/api/events");
            if (response && response->status == 200) {
                std::cout << "GET /api/events passed: " << response->body << std::endl;
            } else {
                std::cout << "GET /api/events failed with status: " << (response ? response->status : -1) << std::endl;
            }
        }

        // Test POST /api/events
        {
            std::string newEvent = R"({"id":1, "description":"Test Event"})";
            auto response = client.Post("/api/events", newEvent, "application/json");
            if (response && response->status == 201) {
                std::cout << "POST /api/events passed" << std::endl;
            } else {
                std::cout << "POST /api/events failed with status: " << (response ? response->status : -1) << std::endl;
            }
        }

        // Test GET /api/events after adding a new event
        {
            auto response = client.Get("/api/events");
            if (response && response->status == 200) {
                std::cout << "GET /api/events (after POST) passed: " << response->body << std::endl;
            } else {
                std::cout << "GET /api/events (after POST) failed with status: " << (response ? response->status : -1) << std::endl;
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "Exception occurred: " << ex.what() << std::endl;
    }

    // Clean up
    serverThread.join();
}

int main() {
    std::cout << "Starting server tests..." << std::endl;

    testServer();

    std::cout << "Tests completed." << std::endl;
    return 0;
}