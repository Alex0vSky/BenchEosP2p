// src\Emulation\Statistics.h - statistics of emulation
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Emulation { 
struct Statistics {
	// TODO(alex): upstream? downstream?
	void fromFirstToSecondBytes(uint32_t) {
		// TODO(alex): timeline, put data and current time
	}
	void fromSecondToFirstBytes(uint32_t) {
	}
	void lostToSecond() {
		// TODO(alex): timeline, put data and current time
	}
	void lostToFirst() {
	}
	void pingLagDuration(timer_resolution_t) {
	}
	void bandwidthDowngradeBytes(uint32_t) {
	}
};
} // namespace syscross::BenchP2p::Emulation
