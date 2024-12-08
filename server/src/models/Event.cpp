#include <chrono>
#include <sstream>
#include <string>
#include "Orderbook.h"
#include "Types.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

using EventId = int;

class Event {
private:
    EventId id;
    std::string description;
    Orderbook orderbook;

    static std::chrono::system_clock::time_point stringToTimePoint(const std::string& date) {
        std::tm tm = {};
        std::istringstream ss(date);
        ss >> std::get_time(&tm, "%d/%m/%Y");
        if (ss.fail()) {
            throw std::runtime_error("Failed to parse date: " + date);
        }
        time_t time = std::mktime(&tm);        
        return std::chrono::system_clock::from_time_t(time);
    }
public:

    Event() {
        this->id = 0;
        this->description = "";
    }

    Event(EventId id, std::string description) {
        this->id = id;
        this->description = description;
    }

    EventId getId() const {
        return id;
    }

    std::string getDescription() const {
        return description;
    }

    Orderbook& getOrderbook() {
        return orderbook;
    }

    json levelsToJson(LevelInfos level) const {
        json levelsJson = json::array();
        for (auto levelInfo : level) {
            levelsJson.push_back({
                {"price", levelInfo.price},
                {"quantity", levelInfo.quantity}
            });
        }
        return levelsJson;
    }

    /*
        To json
     */
    json toString() const {

        json orderInfosJson = {
            {"bids", levelsToJson(orderbook.getOrderInfos().getBids())},
            {"asks", levelsToJson(orderbook.getOrderInfos().getAsks())}
        };

        return {
            {"id", id},
            {"description", description},
            {"orderbook", orderInfosJson}
        };
    }

    static Event fromJson(json json, EventId id) {
        return Event(id, json["description"]);
    }
};


