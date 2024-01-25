// src\BrokerMultiple.h - peers connect logic
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p {
class BrokerMultiple final : public IBrokerMulti {
	tcp::socket *m_server = nullptr;
	mutable peers_t m_clients;
	peers_t *getClients() const override {
		return &m_clients;
	}
	void remove(tcp::socket *socket) override {
		auto it = std::find( m_clients.begin( ), m_clients.end( ), socket );
		if ( m_clients.end( ) != it ) 
			m_clients.erase( it );
	}
	void drop() override {
		for ( auto &cl : m_clients )
			cl ->close( );
		m_clients.clear( );
		LOG( "[~] drop all clients" );
	}
	void setServer(tcp::socket *server) override {
		m_server = server;
	}

public:
	static awaitable listener(tcp::acceptor acceptor, config_t config) {
		namespace Multiple = Behavior::Multiple;
		BrokerMultiple broker;
		Multiple::Negotiator::serverDetector_t wptrDetector;
		while ( true ) {
			auto [e, socket] = co_await acceptor.async_accept( c_tuple );
			if ( e ) {
				LOG( "Accept failed: '%s'", e.message( ).c_str( ) );
				continue;
			}
			if ( !wptrDetector.expired( ) ) {
				if ( wptrDetector.lock( ) ->isServerReady( ) ) {
					tcp::socket *other = Multiple::Client::create( 
						std::move( socket ), &broker, broker.m_server, config )
						->start( );
					broker.m_clients.push_back( other );
				}
				else {
					co_await Net::Communicator::writeCommand( socket, Command::Multi::SrvNeg );
					continue;
				}
			} else {
				wptrDetector = Multiple::Negotiator::create( std::move( socket ), &broker, config );
			}
		}
	}
};
} // namespace syscross::BenchP2p
