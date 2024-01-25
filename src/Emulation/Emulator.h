// src\Emulation\Emulator.h - emergency emulator
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Emulation { 
class Emulator {
	config_t m_config;
	Statistics m_statistics;
	size_t m_counter = 0;
	bool lostPacket() {
		return false;
	}
	bool longPing(tcp::socket::executor_type const&) { // or // steady_timer timer(c o_await this_coro::executor);
		return false;
	}
	bool bandwidthAffect(uint32_t KbPerSecond = MAXDWORD32) {
		(void)KbPerSecond;
		return false;
	}
	bool resetConnection() {
		return false;
	}
	bool connectionDenyDuration(timer_resolution_t) {
		return false;
	}

public:
	Emulator(config_t config) : 
		m_config( config )
	{}
	bool handle(std::size_t dataInBytes) {
		(void)dataInBytes;
		++m_counter;
		if ( m_config.lossOfEveryN ) {
			if ( !( m_counter % m_config.lossOfEveryN.value( ) ) )  {
				m_statistics.add_lossOfEveryN( );
				return true;
			}
		}
		m_statistics.add_ordinary( );
		return false;
	}
};
} // namespace syscross::BenchP2p::Emulation
