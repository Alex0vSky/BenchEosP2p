// src\Emulation\Statistics.h - statistics of emulation
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Emulation { 
struct Statistics {
	static constexpr auto now = std::chrono::steady_clock::now;
	struct Data {
		const Kind m_kind;
		const std::chrono::steady_clock::time_point m_timePoint;
	};
	std::deque< Data > m_data;

	void add_(Kind kind) {
		m_data.push_back( { kind, now( ) } );
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
	void add_longPing() {
		add_( Kind::longPing );
	}
	void add_bandwidth() {
		add_( Kind::bandwidth );
	}
};
} // namespace syscross::BenchP2p::Emulation
