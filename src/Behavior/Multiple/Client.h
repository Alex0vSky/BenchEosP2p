// src\Behavior\Multiple\Client.h - single server and multiple clients
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Behavior::Multiple {
class Client final : public Net::Base, public std::enable_shared_from_this< Client >, public Net::PipeSpawner< Client > {
	struct Private{};
	IBrokerMulti *m_broker = nullptr;
	peers_t m_singlePeer;

	peers_t *getPeers(cref_data_t data, Command::type *command) override {
		(void)data, command;
		return &m_singlePeer;
	}

public:
	Client(Private, tcp::socket &&socket, IBrokerMulti *broker, tcp::socket *server, config_t config) : 
		Base( std::move( socket ), config )
		, m_broker( broker )
		, m_singlePeer{ server }
	{}
	~Client() {
		m_broker ->remove( getSocket( ) );
	}
    static auto create(tcp::socket &&socket, IBrokerMulti *broker, tcp::socket *server, config_t config) {
		return std::make_shared< Client >( 
			Private( ), std::move( socket ), broker, server, config );
    }
};
} // namespace syscross::BenchP2p::Behavior::Multiple
