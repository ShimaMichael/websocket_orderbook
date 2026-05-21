#ifndef HELPER_HPP
#define HELPER_HPP


#include <string>
#include <iostream>
#include <boost/json.hpp>

#include "../messagequeue/messagequeue.hpp"
#include "../orderbook/orderbook.hpp"
#include "../network/websocket.hpp"

namespace json = boost::json;

/*** 
 * Processes a websocket JSON update and applies bid and ask changes to the order book.
 * @param obj Parsed JSON object containing bid updates under "b" and ask updates under "a".
 * @param order_book OrderBook instance to update with parsed price levels.
 * @return No return value.
***/
void processObject(const json::object& obj, OrderBook& order_book);

/*** 
 * Parses a decimal JSON string into a fixed-point signed integer.
 * @param value JSON string containing a non-negative decimal number.
 * @param target_decimals Number of fractional decimal places to preserve.
 * @return Fixed-point integer representation of the input value.
***/
int64_t parse_string_to_int64(const boost::json::string& value, int target_decimals);

/*** 
 * Prints the best bid and ask levels in human-readable decimal format.
 *  param level Pair containing the best bid as first and best ask as second.
 * @return No return value.
***/
void print_order_book(std::pair<OrderLevel, OrderLevel>& level);

/**
 * Continuously processes order book updates from the network queue and applies them to the order book.
 * @param is_running Reference to a boolean flag that controls the loop execution.
 * @param network_queue Reference to the MessageQueue instance that receives raw JSON updates from the network thread.
 * @param order_book Reference to the OrderBook instance that will be updated with parsed price levels.
 * @return No return value.
 */
void order_book_processing_loop(bool& is_running, MessageQueue& network_queue, OrderBook& order_book);

/**
 * Continuously reads messages from the websocket connection and pushes them to the network queue for processing.       
 * @param ws Reference to the active websocket connection to read messages from.
 * @param is_running Reference to a boolean flag that controls the loop execution.
 * @param network_queue Reference to the MessageQueue instance that will receive raw JSON updates for processing.
 * @return No return value.
 */
void network_io_loop(websocket_connection& ws, bool& is_running, MessageQueue& network_queue);

#endif // HELPER_HPP
