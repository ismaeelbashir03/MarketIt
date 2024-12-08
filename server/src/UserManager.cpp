#include <map>
#include "models/User.cpp"

class UserManager {
private:
    std::map<int, User> users;
public:
    void createUser(User u) {
        users[u.getId()] = u;
    }

    User getUser(int id) {
        return users[id];
    }

    void addOrder(int user_id, std::string order) {
        users[user_id].addOrder(order);
    }
};