// src\Behavior\Multiple\Server.h - single server and multiple clients
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Behavior::Multiple {
class Server final : public Net::Base, public std::enable_shared_from_this< Server >, public Net::PipeSpawner< Server > {
	struct Private{};
	peers_t *m_peers = nullptr;
	IBrokerMulti *m_broker = nullptr;

	peers_t *getPeers(cref_data_t, Command::type *command) override {
		if ( !m_peers ->size( ) ) 
			return *command = Command::Multi::WaitCl, m_peers;
		return m_peers;
	}

public:
	Server(Private, tcp::socket &&socket, IBrokerMulti *broker, config_t config) : 
		Base( std::move( socket ), config )
		, m_broker( broker )
		, m_peers( broker ->getClients( ) )
	{}
	~Server() {
		m_broker ->drop( );
	}
    static auto create(tcp::socket &&socket, IBrokerMulti *broker, config_t config) {
		return std::make_shared< Server >( 
			Private( ), std::move( socket ), broker, config );
    }
};
} // namespace syscross::BenchP2p::Behavior::Multiple
