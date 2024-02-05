// src\Behavior\Pair.h - clients pair
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Behavior { 
class Pair final : public Net::Base, public std::enable_shared_from_this< Pair >, public Net::PipeSpawner< Pair > {
	struct Private{};
	peers_t m_singlePeer;
	IBrokerPair *m_broker = nullptr;

	peers_t *getPeers(cref_data_t, Command::type *command) override {
		if ( !m_singlePeer.size( ) ) 
			m_singlePeer.push_back( m_broker ->getPair( ) );
		return &m_singlePeer;
	}

public:
	Pair(Private, socket_t socket, socket_t pair, IBrokerPair *broker, config_t config) : 
		Base( socket, config )
		, m_broker( broker )
		, m_singlePeer( pair ?peers_t{ pair } :peers_t{ } )
	{}
	~Pair() {
		m_broker ->drop( );
	}
    static auto create(socket_t socket, socket_t pair, IBrokerPair *broker, config_t config) {
		return std::make_shared< Pair >( 
			Private( ), socket, pair, broker, config );
    }
};
} // namespace syscross::BenchP2p::Behavior
