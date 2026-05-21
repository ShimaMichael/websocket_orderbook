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

/** 
 * Owns a TLS websocket connection to a market data endpoint.
 * @param None.
 * @return No return value.
*/
class WebSocketClient {
    public:
        /**
         * Creates a websocket client and configures the SSL context.
         * @param None.
         * @return No return value.
        */
        WebSocketClient();

        /***
         * Destroys the websocket client and owned networking resources.
         * @param None.
         * @return No return value.
        */
        ~WebSocketClient() = default;    

        /**
         * Opens the TCP, TLS, and websocket handshakes to the configured endpoint.
         * @param host Websocket host name to resolve and connect to.
         * @param port Service port to connect to.
         * @param target Websocket request target used during the handshake.
         * @return No return value.
        */
        void connect(std::string host = "ws.gemini.com", std::string port = "443", std::string target = "/?snapshot=-1");

        /**  
         * Sends the order book depth subscription message over the active websocket.
         * @param None.
         * @return No return value.
        */
        void subscribe();

        /** 
         * Provides access to the underlying websocket stream.
         * @param None.
         * @return Reference to the active websocket connection.
        */
        websocket_connection& get_connection() {
            return ws_;
        }

        
    private:
        net::io_context ioc_;
        ssl::context ctx_{ssl::context::tlsv12_client};
        tcp::resolver resolver_{ioc_};
        websocket_connection ws_{ioc_, ctx_};

        /**
         * Configures websocket timeout and user-agent options before handshaking.
         * @param None.
         * @return No return value.
        */
        void set_options();

};

#endif // WEBSOCKET_HPP
