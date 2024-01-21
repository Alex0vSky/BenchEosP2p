// src\SignalSet.h - stop by Ctrl+C
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p { 
class SignalSet {
	boost::asio::signal_set m_signals;

public:
	SignalSet(boost::asio::io_context &io_context) :
		m_signals( io_context, SIGINT, SIGTERM )
	{}
	void ordinary() {
		m_signals.async_wait(
			[&](auto, auto) { 
				auto context = static_cast<boost::asio::io_context *>
					( &m_signals.get_executor( ).context( ) );
				context ->stop( );
			});
		LOG( "[~] press [Ctr+C] to exit" );
	}
};
} // namespace syscross::BenchP2p
