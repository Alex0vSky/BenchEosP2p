// src\Behavior\Pair.h - clients pair
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Behavior { 
class Pair final : public Net::Base, public std::enable_shared_from_this< Pair >, public Net::PipeSpawner< Pair > {
	struct Private{};
	IBrokerPair *m_broker = nullptr;
	peers_t m_singlePeer;

	peers_t *getPeers(cref_data_t, Command::type *command) override {
		*command = Command::Pair::NoPair;
		if ( !m_singlePeer.size( ) ) 
			if ( tcp::socket *socket = m_broker ->getPair( ) ) 
				m_singlePeer.push_back( socket );
		return &m_singlePeer;
	}

public:
	Pair(Private, tcp::socket &&socket, tcp::socket *pair, IBrokerPair *broker, config_t config) : 
		Base( std::move( socket ), config )
		, m_broker( broker )
		, m_singlePeer{ pair ?peers_t{ pair } :peers_t{ } }
	{}
	~Pair() {
		m_broker ->drop( );
	}
    static auto create(tcp::socket &&socket, tcp::socket *pair, IBrokerPair *broker, config_t config) {
		return std::make_shared< Pair >( 
			Private( ), std::move( socket ), pair, broker, config );
    }
};
} // namespace syscross::BenchP2p::Behavior
