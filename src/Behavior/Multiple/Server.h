// src\Behavior\Multiple\Server.h - single server and multiple clients
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Behavior::Multiple {
class Server final : public Net::Base, public std::enable_shared_from_this< Server >, public Net::PipeSpawner< Server > {
	peers_t *m_peers = nullptr;
	IBrokerMulti *m_broker = nullptr;
	bool m_bAcceptedServer = false;

	peers_t *getPeers(cref_data_t data, Command::type *command) override {
		if ( !m_peers ->size( ) ) {
			if ( m_bAcceptedServer ) 
				return *command = Command::Multi::WaitCl, m_peers;
			if ( Command::Multi::ImServ == data ) 
				*command = Command::Multi::Accept, m_bAcceptedServer = true;
			else
				*command = Command::Multi::Denied;
			return m_peers;
		}
		return m_peers;
	}

public:
	Server(tcp::socket &&socket, IBrokerMulti *broker) : 
		Base( std::move( socket ) )
		, m_broker( broker )
		, m_peers( broker ->getClients( ) )
	{}
	~Server() {
		m_broker ->drop( );
	}
};
} // namespace syscross::BenchP2p::Behavior::Multiple
