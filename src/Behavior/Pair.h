// src\Behavior\Pair.h - clients pair
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Behavior { 
class Pair final : public Net::Base, public std::enable_shared_from_this< Pair > {
	tcp::socket *m_socketPair = nullptr;
	IBroker *m_broker = nullptr;

	tcp::socket *getPeer(Command::type *command) override {
		if ( !m_socketPair ) 
			if ( !( m_socketPair = m_broker ->getPair( ) ) ) 
				return *command = Command::NoPair, nullptr;
		return m_socketPair;
	}

public:
	Pair(tcp::socket &&socket, tcp::socket *socketPair, IBroker *broker = nullptr) : 
		Base( std::move( socket ) )
		, m_socketPair( socketPair )
		, m_broker( broker )
	{}

	tcp::socket *start() {
		co_spawn( getSocket( ) ->get_executor( )
				, [self = shared_from_this( )] { 
					return self ->pipe( );
				}
				, c_detached 
			);
		return getSocket( );
	}
};
} // namespace syscross::BenchP2p::Behavior
