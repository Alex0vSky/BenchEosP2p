// src\Broker.h - peers connect logic
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p {
class Broker final : public IBrokerPair {
	tcp::socket *m_first = nullptr, *m_second = nullptr;
	tcp::socket *getPair() const override {
		return m_second;
	}
	void drop() override {
		if ( m_first )
			m_first ->close( );
		if ( m_second )
			m_second ->close( );
		m_first = m_second = nullptr;
		LOG( "[~] drop pair" );
	}

public:
	static awaitable listener(tcp::acceptor acceptor) {
		Broker broker;
		while ( true ) {
			auto [e, socket] = co_await acceptor.async_accept( c_tuple );
			if ( e ) {
				LOG( "Accept failed: '%s'", e.message( ).c_str( ) );
				continue;
			}
			if ( broker.m_second ) {
				co_await Net::Communicator::writeCommand( socket, Command::Pair::NoMore );
				continue;
			}
			if ( !broker.m_first ) 
				broker.m_first = Behavior::Pair::create( 
					std::move( socket ), nullptr, &broker )
					->start( );
			else
				broker.m_second = Behavior::Pair::create( 
					std::move( socket ), broker.m_first, &broker )
					->start( );
		}
	}
};
} // namespace syscross::BenchP2p
