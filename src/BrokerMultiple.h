// src\BrokerMultiple.h - peers connect logic
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p {
class BrokerMultiple final : public IBrokerMulti {
	tcp::socket *m_first = nullptr;
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
		m_first = nullptr;
		LOG( "[~] drop all clients" );
	}

public:
	static awaitable listener(tcp::acceptor acceptor) {
		BrokerMultiple broker;
		while ( true ) {
			auto [e, socket] = co_await acceptor.async_accept( c_tuple );
			if ( e ) {
				LOG( "Accept failed: '%s'", e.message( ).c_str( ) );
				continue;
			}
			if ( !broker.m_first ) 
				broker.m_first = std::make_shared< Behavior::Multiple::Server >
					( std::move( socket ), &broker )
					->start( );
			else {
				tcp::socket *other = std::make_shared< Behavior::Multiple::Client >
					( std::move( socket ), &broker, broker.m_first )
					->start( );
				broker.m_clients.push_back( other );
			}
		}
	}
};
} // namespace syscross::BenchP2p
