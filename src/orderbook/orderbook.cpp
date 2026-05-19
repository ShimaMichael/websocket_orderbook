#include "orderbook.hpp"

#include <iostream>

/*** Updates the bid side of the order book with a single price level change.
@param price Bid price scaled by 100.
@param quantity Bid quantity scaled by 100000000. A value of 0 removes the level.
@return No return value.
***/
void OrderBook::update_bid(int64_t price, int64_t quantity){
    update_order(price, quantity, best_bids_heap_, bids_);
}

/*** Updates the ask side of the order book with a single price level change.
@param price Ask price scaled by 100.
@param quantity Ask quantity scaled by 100000000. A value of 0 removes the level.
@return No return value.
***/
void OrderBook::update_ask( int64_t price, int64_t quantity){
    update_order(price, quantity, best_asks_heap_, asks_);
}

/*** Returns the highest active bid currently stored in the order book.
@param None.
@return Best bid as an OrderLevel, or {0, 0} when no bid is available.
***/
OrderLevel OrderBook::get_best_bid() {
    return get_best_level(best_bids_heap_, bids_);
}

/*** Returns the lowest active ask currently stored in the order book.
@param None.
@return Best ask as an OrderLevel, or {0, 0} when no ask is available.
***/
OrderLevel OrderBook::get_best_ask() {
    return get_best_level(best_asks_heap_, asks_);
}

