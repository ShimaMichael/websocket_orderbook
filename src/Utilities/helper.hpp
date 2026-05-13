#ifndef HELPER_HPP
#define HELPER_HPP


#include <string>
#include <iostream>
#include <boost/json.hpp>

#include "../orderbook/orderbook.hpp"

namespace json = boost::json;

void processObject(const json::object& obj, OrderBook& order_book);

#endif // HELPER_HPP