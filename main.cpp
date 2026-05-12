#include <iostream>
#include <string>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>

#include <openssl/ssl.h>
#include <openssl/err.h>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

int main() {
    try {
        net::io_context ioc;

        ssl::context ctx{ssl::context::tlsv12_client};

        ctx.set_verify_mode(ssl::verify_none);

        tcp::resolver resolver{ioc};

        websocket::stream<beast::ssl_stream<tcp::socket>> ws{ioc, ctx};

        
        std::string host = "ws.gemini.com";
        std::string port = "443";
        std::string target = "/?snapshot=-1";

        
        auto const results = resolver.resolve(host, port);

        
        net::connect(
            beast::get_lowest_layer(ws),
            results
        );

        
        if(!SSL_set_tlsext_host_name(
            ws.next_layer().native_handle(),
            host.c_str()))
        {
            throw beast::system_error(
                beast::error_code(
                    static_cast<int>(::ERR_get_error()),
                    net::error::get_ssl_category()
                )
            );
        }

        ws.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));
        ws.set_option(websocket::stream_base::decorator(
            [](websocket::request_type& req) {
                req.set(boost::beast::http::field::user_agent, "Boost.Beast WebSocket Client");
            }
        ));

        
        ws.next_layer().handshake(ssl::stream_base::client);
        
        ws.handshake(host, target);

        std::cout
            << "Connected to Gemini depth stream"
            << std::endl;

        std::string sub = R"({
            "method":"SUBSCRIBE",
            "params":["btcusd@depth@100ms"],
            "id":1
        })";

        ws.write(net::buffer(std::string(sub)));

        for (;;) {
            beast::flat_buffer buffer;

            ws.read(buffer);

            std::cout
                << beast::make_printable(buffer.data())
                << std::endl;
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

