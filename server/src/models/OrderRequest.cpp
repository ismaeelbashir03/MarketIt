#include "Order.h"

class OrderRequest {    
private:
    OrderId orderId;
    Price price;
    Quantity quantity;
    Side side;
    OrderType orderType;

public:
    OrderRequest(OrderId orderId, Price price, Quantity quantity, Side side, OrderType orderType) {
        this->orderId = orderId;
        this->price = price;
        this->quantity = quantity;
        this->side = side;
        this->orderType = orderType;
    }

    /*
        To json
     */
    std::string toString() {
        return "";
    }
};