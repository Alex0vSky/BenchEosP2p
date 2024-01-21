// src\Broker.h - peers connect logic
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p {
class Broker final : public IBroker {
	tcp::socket *m_first = nullptr, *m_second = nullptr;
	tcp::socket *getPair() const override {
		return m_second;
	}

public:
	static awaitable listener(tcp::acceptor acceptor) {
		Broker broker;
		for (;;) {
			auto [e, socket] = co_await acceptor.async_accept( c_tuple );
			if ( e ) {
				LOG( "Accept failed: '%s'", e.message( ).c_str( ) );
				continue;
			}
			if ( broker.m_second ) {
				co_await Net::Communicator::writeCommand( socket, Command::NoMore );
				continue;
			}
			if ( !broker.m_first ) 
				broker.m_first = std::make_shared< client_t >( std::move( socket ), nullptr, &broker )
					->start( );
			else
				broker.m_second = std::make_shared< client_t >( std::move( socket ), broker.m_first )
					->start( );
		}
	}
};
} // namespace syscross::BenchP2p
