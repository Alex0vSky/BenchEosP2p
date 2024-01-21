// src\Emulation\Emulator.h - indeed
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Emulation { 
class Emulator {
	Emulation::Statistics m_statisticCalc;
	Emulation::Config m_config;
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
	bool handle(std::size_t dataInBytes) {
		(void)dataInBytes;
		return false;
	}
};
} // namespace syscross::BenchP2p::Emulation
