// src\aliases.h - indeed
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p { 
#ifdef BOOST_ASIO_HAS_CO_AWAIT
using awaitable = boost::asio::awaitable<void>;
#endif // BOOST_ASIO_HAS_CO_AWAIT
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

#ifdef BOOST_ASIO_HAS_CO_AWAIT
// Not use exception
#	ifndef c_tuple
static constexpr auto c_tuple = as_tuple( boost::asio::use_awaitable_t{ } );
#	endif // c_tuple_awaitable
#endif // BOOST_ASIO_HAS_CO_AWAIT

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

typedef std::shared_ptr< tcp::socket > socket_t;
typedef std::deque< socket_t > peers_t;

// @insp https://stackoverflow.com/questions/76220547/boost-asio-implementing-events
using channel_t = boost::asio::experimental::channel< void(boost::system::error_code, int) >;

typedef const Emulation ::Config config_t;

//typedef tcp::socket tcp_socket_t;

} // namespace syscross::BenchP2p
