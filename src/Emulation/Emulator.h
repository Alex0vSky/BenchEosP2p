// src\Emulation\Emulator.h - emergency emulator
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Emulation { 
class Emulator {
	config_t m_config;
	tcp::socket::executor_type const& m_executor;
	Statistics m_statistics;
	size_t m_counter = 0;
	// TODO(alex): makeme
	bool isBandwidthAffect_(uint32_t currentDataInBytes) {
		if ( !m_config.bandwidthInKBytes )
			return false;
		uint32_t bandwidthInKBytes = m_config.bandwidthInKBytes.value( );
		(void)currentDataInBytes;
		(void)bandwidthInKBytes;
		return false;
	}

public:
	Emulator(config_t config, tcp::socket::executor_type const& executor) : 
		m_config( config )
		, m_executor( executor )
	{}
	boost::asio::awaitable< bool > handle(std::size_t dataInBytes) {
		++m_counter;
		if ( m_config.lossOfEveryN ) {
			if ( !( m_counter % m_config.lossOfEveryN.value( ) ) ) {
				m_statistics.add_lossOfEveryN( );
				co_return true;
			}
		}
		// @insp https://stackoverflow.com/questions/70577818/c20-coroutines-with-boost-awaitable-custom-suspension
		if ( m_config.longPingDuration_milli ) {
			boost::posix_time::milliseconds duration( m_config.longPingDuration_milli.value( ) );
			boost::asio::deadline_timer timer( m_executor, duration );
			co_await timer.async_wait( use_awaitable );
			m_statistics.add_longPing( );
			co_return false;
		}
		if ( isBandwidthAffect_( dataInBytes ) ) {
			m_statistics.add_bandwidth( );
			co_return false;
		}
		m_statistics.add_ordinary( );
		co_return false;
	}
};
} // namespace syscross::BenchP2p::Emulation
