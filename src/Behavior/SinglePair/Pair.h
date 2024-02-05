// src\Behavior\SinglePair\Pair.h - clients pair
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Behavior::SinglePair { 
class Pair final : public Net::Base, public std::enable_shared_from_this< Pair >, public Net::PipeSpawner< Pair > {
	struct Private{};
	peers_t m_singlePeer;
	IBrokerPair *m_broker = nullptr;

	boost::asio::awaitable< peers_t * > getPeers(Command::type *command) override {
		if ( !m_singlePeer.size( ) ) {
			socket_t socket;
			// TODO(alex): TO
			while ( !( socket = m_broker ->getPair( ) ) ) 
				co_await sleep( boost::posix_time::milliseconds{ 300 } );
			m_singlePeer.push_back( socket );
		}
		co_return &m_singlePeer;
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
} // namespace syscross::BenchP2p::Behavior::SinglePair
