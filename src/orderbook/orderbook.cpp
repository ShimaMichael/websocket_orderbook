#include "orderbook.hpp"
#include <iostream>


void OrderBook::load_snapshot(const std::vector<std::pair<std::string, std::string>>& bids,
                    const std::vector<std::pair<std::string, std::string>>& asks) {
    bids_.clear();
    asks_.clear();

    for (const auto& bid : bids) {
        this->bids_[std::stod(bid.first)] = std::stod(bid.second);
    }
    for (const auto& ask : asks) {
        this->asks_[std::stod(ask.first)] = std::stod(ask.second);
    }
}

void OrderBook::update_bid(const std::string& price, const std::string& quantity){
    if (std::stod(quantity) == 0.0) {
        this->bids_.erase(std::stod(price));
    } else {
        this->bids_[std::stod(price)] = std::stod(quantity);
    }
}

void OrderBook::update_ask(const std::string& price, const std::string& quantity){
    if (std::stod(quantity) == 0.0) {
        this->asks_.erase(std::stod(price));
    } else {
        this->asks_[std::stod(price)] = std::stod(quantity);
    }
}
