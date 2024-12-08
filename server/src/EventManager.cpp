#include <iostream>
#include <map>
#include <string>
#include <mutex>
#include "Orderbook.h"
#include "Trade.h"
#include "models/Event.cpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class EventManager {
private:
    std::map<EventId, Event> events; 
    // atomic for id generation
    std::atomic<int> nxt_id{0}; 
    // lock for accessing map   
    std::mutex mtx;              

public:
    void createEvent(const Event& e) {
        events[e.getId()] = e;
    }

    void createEvent(const std::string& body) {
        std::lock_guard<std::mutex> lock(mtx); 
        try {
            auto jsonData = json::parse(body); 

            if (!jsonData.contains("description")) {
                throw std::invalid_argument("Missing 'description' field");
            }

            Event e = Event::fromJson(jsonData, nxt_id++);
            events[e.getId()] = e;
        } catch (const std::exception& ex) {
            std::cerr << "Error in createEvent: " << ex.what() << std::endl;
            throw; 
        }
    }

    std::string getEvents() {
        std::lock_guard<std::mutex> lock(mtx);
        json res = json::array();

        for (const auto& [id, event] : events) {
            res.push_back(event.toString());
        }

        return res.dump();
    }

    int addOrder(const std::string& body) {
        std::lock_guard<std::mutex> lock(mtx);
        try {
            auto jsonData = json::parse(body);

            if (!jsonData.contains("event_id")) {
                throw std::invalid_argument("Missing 'event_id' field");
            }

            int event_id = jsonData["event_id"];
            if (events.find(event_id) == events.end()) {
                throw std::invalid_argument("Event not found");
            }
            Orderbook& orderbook = events[event_id].getOrderbook();
            OrderConfirmation confirmation = orderbook.addOrder(jsonData["price"], jsonData["quantity"], jsonData["side"], jsonData["orderType"]);
            if (confirmation.first == 0) {
                return -1;
            }
            int order_id = confirmation.first;
            return order_id;
        } catch (const std::exception& ex) {
            std::cerr << "Error in addOrder: " << ex.what() << std::endl;
            throw;
        }
    }
};