#include <iostream>
#include <string>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/json.hpp>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "./src/network/websocket.hpp"
#include "./src/orderbook/orderbook.hpp"
#include "./src/Utilities/helper.hpp"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;
namespace json = boost::json;

OrderBook order_book;
WebSocketClient ws_client;


int main() {
    try {
        ws_client.connect();
        ws_client.suscribe();
        websocket_connection& ws = ws_client.get_connection();

        int count = 0;
        for (;;) {
            beast::flat_buffer buffer;

            ws.read(buffer);

            std::string data = beast::buffers_to_string(buffer.data());
            auto parsed = json::parse(data);
            auto obj = parsed.as_object();

            std::cout << "Received update: " << obj << std::endl;

            processObject(obj, order_book);         
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

