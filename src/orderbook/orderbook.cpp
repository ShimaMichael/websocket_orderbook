#include "orderbook.hpp"

#include <iostream>

void OrderBook::update_bid(int64_t price, int64_t quantity){
    update_order(price, quantity, best_bids_heap_, bids_);
}

void OrderBook::update_ask( int64_t price, int64_t quantity){
    update_order(price, quantity, best_asks_heap_, asks_);
}

OrderLevel OrderBook::get_best_bid() {
    return get_best_level(best_bids_heap_, bids_);
}

OrderLevel OrderBook::get_best_ask() {
    return get_best_level(best_asks_heap_, asks_);
}

