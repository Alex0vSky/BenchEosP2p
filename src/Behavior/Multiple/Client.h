// src\Behavior\Multiple\Client.h - single server and multiple clients
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Behavior::Multiple {
class Client final : public Net::Base, public std::enable_shared_from_this< Client >, public Net::PipeSpawner< Client > {
	IBrokerMulti *m_broker = nullptr;
	peers_t m_singlePeer;

	peers_t *getPeers(cref_data_t data, Command::type *command) override {
		(void)data, command;
		return &m_singlePeer;
	}

public:
	Client(tcp::socket &&socket, IBrokerMulti *broker, tcp::socket *server) : 
		Base( std::move( socket ) )
		, m_broker( broker )
		, m_singlePeer{ server }
	{}
	~Client() {
		m_broker ->remove( getSocket( ) );
	}
};
} // namespace syscross::BenchP2p::Behavior::Multiple
