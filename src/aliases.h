// src\aliases.h - indeed
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p { 
using awaitable = boost::asio::awaitable<void>;
using tcp = boost::asio::ip::tcp;
// aliases
static constexpr auto c_detached = boost::asio::detached;
template <typename... Args>
inline auto as_tuple(Args&&... args) {
	return boost::asio::as_tuple( std::forward< Args >( args )... );
}
template <typename... Args>
inline auto buffer(Args&&... args) -> decltype( boost::asio::buffer( std::forward< Args >( args)... ) ) {
	return boost::asio::buffer( std::forward< Args >( args )... );
}

// Not use exception
#ifndef c_tuple
static constexpr auto c_tuple = as_tuple( boost::asio::use_awaitable_t{ } );
#endif // c_tuple_awaitable

namespace Emulation { 
struct Config;
} // namespace Behavior
namespace Behavior { 
class Pair;
namespace Multiple { 
class Server;
class Client;
} // namespace Multiple
} // namespace Behavior

static const uint32_t c_dataInBytes = 1024;
typedef char data_t[ c_dataInBytes ];
typedef data_t const& cref_data_t;

typedef std::deque< tcp::socket * > peers_t;

typedef std::chrono::milliseconds timer_resolution_t;

typedef const Emulation ::Config config_t;

//typedef tcp::socket tcp_socket_t;

} // namespace syscross::BenchP2p
