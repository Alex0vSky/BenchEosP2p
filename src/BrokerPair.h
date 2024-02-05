// src\BrokerPair.h - peers connect logic
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p {
class BrokerPair final : public IBrokerPair {
	socket_t m_first, m_second;
	socket_t getPair() const override {
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
	socket_t &getFirstRef() override {
		return m_first;
	}
	socket_t &getSecondRef() override {
		return m_second;
	}

public:
	[[nodiscard]] static awaitable listener(tcp::acceptor acceptor, config_t config) {
		using Negotiator = Behavior::SinglePair::Negotiator;
		BrokerPair broker;
		Negotiator::negotiator_t wptrFirst, wptrSecond;
		while ( true ) {
			auto [e, socket] = co_await acceptor.async_accept( c_tuple );
			if ( e ) {
				LOG( "Accept failed: '%s'", e.message( ).c_str( ) );
				continue;
			}
			if ( !wptrSecond.expired( ) ) {
				if ( wptrSecond.lock( ) ->isReady( ) ) 
					co_await Net::Communicator::writeCommand( socket, Command::Pair::NoMore );
				else
					co_await Net::Communicator::writeCommand( socket, Command::Pair::Negott );
				continue;
			}
			socket_t socket_sptr = std::make_shared< tcp::socket >( std::move( socket ) );
			if ( wptrFirst.expired( ) ) {
				wptrFirst = Negotiator::create( { socket_sptr, &broker, config, true } );
			}
			else 
				wptrSecond = Negotiator::create( { socket_sptr, &broker, config, false } );
		}
	}
};
} // namespace syscross::BenchP2p
