// src\Net\Communicator.h - data exchange
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Net { 
class Communicator {
	data_t m_data = { };
	boost::system::error_code m_error; 

protected:
	boost::asio::awaitable< std::size_t > readData(tcp::socket &socket) {
		std::size_t n = 0;
		std::tie( m_error, n ) = co_await socket.async_read_some( buffer( m_data ), c_tuple );
		co_return n;
	}
	boost::asio::awaitable< bool > writeData(tcp::socket *socket, std::size_t n) {
		std::size_t unused_ = 0;
		std::tie( m_error, unused_ ) = co_await async_write( *socket, buffer( m_data, n ), c_tuple );
		co_return m_error.failed( );
	}
	auto getError() const {
		return m_error;
	}
	cref_data_t getData() const {
		return m_data;
	}

public:

	static awaitable sleep(boost::posix_time::milliseconds duration) {
		boost::asio::deadline_timer timer( co_await boost::asio::this_coro::executor, duration );
		co_await timer.async_wait( use_awaitable );
	}
	static boost::asio::awaitable< Command::type > readCommand(tcp::socket &socket) {
		Command::type command = { };
		co_await async_read( socket, buffer( command ), c_tuple );
		co_return command;
	}
	static boost::asio::awaitable< boost::system::error_code > writeCommand(tcp::socket &socket, Command::type command) {
		std::size_t unused_ = 0;
		boost::system::error_code error; 
		std::tie( error, unused_ ) = co_await async_write( socket, buffer( command ), c_tuple );
		co_return error;
	}
};
} // namespace syscross::BenchP2p::Net
