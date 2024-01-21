// src\Net\Communicator.h - data exchange
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Net { 
class Communicator {
	char m_data[ 1024 ];
	boost::system::error_code m_error; 

protected:
	boost::asio::awaitable< std::size_t > readData(tcp::socket &socket) {
		std::size_t n = 0;
		std::tie( m_error, n ) = co_await socket.async_read_some( buffer( m_data ), c_tuple );
		co_return n;
	}
	boost::asio::awaitable< bool > writeData(tcp::socket &socket, std::size_t n) {
		std::size_t unused_ = 0;
		std::tie( m_error, unused_ ) = co_await async_write( socket, buffer( m_data, n ), c_tuple );
		co_return !m_error;
	}
	auto getError() const {
		return m_error;
	}

public:
	static boost::asio::awaitable< boost::system::error_code > writeCommand(tcp::socket &socket, Command::type command) {
		std::size_t unused_ = 0;
		boost::system::error_code error; 
		std::tie( error, unused_ ) = co_await async_write( socket, buffer( command ), c_tuple );
		co_return error;
	}
};
} // namespace syscross::BenchP2p::Net