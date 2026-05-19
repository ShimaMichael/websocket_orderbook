#include <iostream>
#include <string>

#include "./src/network/websocket.hpp"
#include "./src/orderbook/orderbook.hpp"
#include "./src/Utilities/helper.hpp"

namespace json = boost::json;

OrderBook order_book;
WebSocketClient ws_client;
std::pair<OrderLevel, OrderLevel> best_levels;


int main() {
    try {
        ws_client.connect();
        ws_client.suscribe();
        websocket_connection& ws = ws_client.get_connection();

        for (;;) {
            beast::flat_buffer buffer;

            ws.read(buffer);

            std::string data = beast::buffers_to_string(buffer.data());
            auto parsed = json::parse(data);
            auto obj = parsed.as_object();

            processObject(obj, order_book);  
            
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

