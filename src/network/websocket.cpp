#include "websocket.hpp"


WebSocketClient::WebSocketClient():
    ioc_(),
    ctx_{ssl::context::tlsv12_client},
    resolver_{ioc_},
    ws_{ioc_, ctx_}
{
    ctx_.set_verify_mode(ssl::verify_none);
}

void WebSocketClient::connect() {
        
    std::string host = "ws.gemini.com";
    std::string port = "443";
    std::string target = "/?snapshot=-1";

    
    auto const results = this->resolver_.resolve(host, port);

    
    net::connect(
        beast::get_lowest_layer(this->ws_),
        results
    );

    
    if(!SSL_set_tlsext_host_name(
        this->ws_.next_layer().native_handle(),
        host.c_str()))
    {
        throw beast::system_error(
            beast::error_code(
                static_cast<int>(::ERR_get_error()),
                net::error::get_ssl_category()
            )
        );
    }

    this->ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));
    this->ws_.set_option(websocket::stream_base::decorator(
        [](websocket::request_type& req) {
            req.set(boost::beast::http::field::user_agent, "Boost.Beast WebSocket Client");
        }
    ));

    
    this->ws_.next_layer().handshake(ssl::stream_base::client);
    
    this->ws_.handshake(host, target);

    std::cout
        << "Connected to Gemini depth stream"
        << std::endl;
}


void WebSocketClient::suscribe() {
    std::string sub = R"({
        "method":"SUBSCRIBE",
        "params":["btcusd@depth@100ms"],
        "id":1
    })";

    this->ws_.write(net::buffer(std::string(sub)));
}