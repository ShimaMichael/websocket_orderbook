#ifndef HELPER_HPP
#define HELPER_HPP


#include <string>
#include <iostream>
#include <boost/json.hpp>

#include "../orderbook/orderbook.hpp"

namespace json = boost::json;

void processObject(const json::object& obj, OrderBook& order_book);

int64_t parse_string_to_int64(const boost::json::string& value, int target_decimals);

void print_order_book(std::pair<OrderLevel, OrderLevel>& level);

#endif // HELPER_HPP