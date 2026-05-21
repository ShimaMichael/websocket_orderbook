#include <iostream>
#include <string>
#include <thread>

#include "./src/network/websocket.hpp"
#include "./src/orderbook/orderbook.hpp"
#include "./src/Utilities/helper.hpp"
#include "./src/messagequeue/messagequeue.hpp"

namespace json = boost::json;

OrderBook order_book;
WebSocketClient ws_client;
std::pair<OrderLevel, OrderLevel> best_levels;
MessageQueue message_queue;
bool is_running = true;
std::string raw_json;

/*** 
 * Connects to the websocket feed, processes order book updates, and prints the best levels.
 * @param None.
 * @return Program exit status code.
***/
int main() {
    try {
        ws_client.connect();
        ws_client.subscribe();
        websocket_connection& ws = ws_client.get_connection();

        std::thread io_thread(network_io_loop, std::ref(ws), std::ref(is_running), std::ref(message_queue));

        for (;;) {
            if (message_queue.pop(raw_json)) {
                auto parsed = json::parse(raw_json);
                const auto& obj = parsed.as_object();

                processObject(obj, order_book);
            }
            
            auto best_bid = order_book.get_best_bid();
            auto best_ask = order_book.get_best_ask();


            if (best_bid == best_levels.first && best_ask == best_levels.second) {
                continue;
            }
            else {
                best_levels = {best_bid, best_ask};
                print_order_book(best_levels);
            }
        }

    }
    catch (const std::exception& e) {
            std::cerr
                << "Error: "
                << e.what()
                << std::endl;
    }

    return 0;
}

