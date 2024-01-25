// src\Emulation\Statistics.h - statistics of emulation
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Emulation { 
struct Statistics {
	static constexpr auto now = std::chrono::steady_clock::now;
	struct Data {
		const std::chrono::steady_clock::time_point m_timePoint;
		const Kind m_kind;
	};
	std::deque< Data > m_data;

	void add_(Kind kind) {
		m_data.push_back( { now( ), kind } );
	}

public:
	Statistics() {
		add_( Kind::begin );
	}
	~Statistics() {
		add_( Kind::finish );
	}
	void add_ordinary() {
		add_( Kind::ordinary );
	}
	void add_lossOfEveryN() {
		add_( Kind::lossOfEveryN );
	}

	// TODO(alex): upstream? downstream?
	void fromFirstToSecondBytes(uint32_t) {
	}
	void fromSecondToFirstBytes(uint32_t) {
	}
	void lostToSecond() {
	}
	void lostToFirst() {
	}
	void pingLagDuration(timer_resolution_t) {
	}
	void bandwidthDowngradeBytes(uint32_t) {
	}
};
} // namespace syscross::BenchP2p::Emulation
