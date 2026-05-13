#include "helper.hpp"

void processObject(const json::object& obj, OrderBook& order_book) {
    if(obj.contains("b")){
        auto bids = obj.at("b").as_array();
        for(const auto& bid:bids){
            auto level = bid.as_array();
            if (level.size() < 2) {
                std::cerr << "Invalid bid data: " << bid << std::endl;
                continue; 
            }
            std::string price = std::string(level[0].as_string());
            std::string quantity = std::string(level[1].as_string());
            order_book.update_bid(price, quantity); 
        }
    }

    if(obj.contains("a")){
        auto asks = obj.at("a").as_array();
        for(const auto& ask : asks){
            auto level = ask.as_array();
            if (level.size() < 2) {
                std::cerr << "Invalid ask data: " << ask << std::endl;
                continue; 
            }
            std::string price = std::string(level[0].as_string());
            std::string quantity = std::string(level[1].as_string());
            order_book.update_ask(price, quantity); 
        }
    }
}