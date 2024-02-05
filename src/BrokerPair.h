// src\BrokerPair.h - peers connect logic
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p {
class BrokerPair final : public IBrokerPair {
	socket_t m_first, m_second;
	bool m_isPaired = false;
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
	bool &isPaired() override {
		return m_isPaired;
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
		using Context = Behavior::SinglePair::Context;
		using Chanels = Behavior::SinglePair::Channels;
		BrokerPair broker;
		Negotiator::negotiator_t wptrFirst, wptrSecond;
		// @insp https://stackoverflow.com/questions/76220547/boost-asio-implementing-events
		Chanels channels( acceptor.get_executor( ) );
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
			socket_t Socket_sptr = std::make_shared< tcp::socket >( std::move( socket ) );
			if ( wptrFirst.expired( ) ) {
				wptrFirst = Negotiator::create( { Socket_sptr, &broker, config, true, channels } );
			}
			else 
				wptrSecond = Negotiator::create( { Socket_sptr, &broker, config, false, channels } );
		}
	}
};
} // namespace syscross::BenchP2p
