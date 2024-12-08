#include <string>
#include <map>
#include "nlohmann/json.hpp"

using Holdings = std::map<int, double>;
using json = nlohmann::json;

class User {
private:
    int id;
    std::string name;
    double score;
    Holdings holdings;
public:
        User() {
            this->id = 0;
            this->name = "";
            this->score = 0;
        }
        User(int id, std::string name) {
            this->id = id;
            this->name = name;
            this->score = 0;
            this->holdings = Holdings();
        }
        int getId() {
            return id;
        }
        std::string getName() {
            return name;
        }
        double getScore() {
            return score;
        }
        json getJson() {
            json j;
            j["id"] = id;
            j["name"] = name;
            j["score"] = score;
            j["holdings"] = holdings;
            return j;
        }
        void addOrder(std::string order) {
            json j = json::parse(order);
            int side = j["side"];
            int quantity = j["quantity"];
            int event_id = j["event_id"];
            if (side == 0) {
                holdings[event_id] += quantity;
            } else {
                holdings[event_id] -= quantity;
            }
        }

};