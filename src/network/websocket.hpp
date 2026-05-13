#ifndef WEBSOCKET_HPP
#define WEBSOCKET_HPP

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

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;

using tcp = boost::asio::ip::tcp;
using websocket_connection = websocket::stream<beast::ssl_stream<tcp::socket>>;

class WebSocketClient {
    public:
        WebSocketClient();
        ~WebSocketClient() = default;    

        void connect();

        void suscribe();

        websocket_connection& get_connection() {
            return ws_;
        }

        
    private:
        net::io_context ioc_;
        ssl::context ctx_{ssl::context::tlsv12_client};
        tcp::resolver resolver_{ioc_};
        websocket_connection ws_{ioc_, ctx_};

        

};

websocket_connection websocket_init();

#endif // WEBSOCKET_HPP