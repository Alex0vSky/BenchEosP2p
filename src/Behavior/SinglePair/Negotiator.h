// src\Behavior\SinglePair\Negotiator.h - negotiator for waiting prepared pairs peer
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Behavior::SinglePair { 
class Negotiator : public std::enable_shared_from_this< Negotiator >, public Net::Communicator {
	struct Private{};
	socket_t m_socket;
	IBrokerPair *const m_broker = nullptr;
	config_t m_config;
	const bool m_isFirst;
	bool m_isReady = false;

	awaitable negotiate() {
		do {
			Command::type command = co_await readCommand( *m_socket );
			if ( Command::Pair::Hello_ != command ) {
				co_await writeCommand( *m_socket, Command::Pair::Wrong_ );
				co_return;
			}
			if ( !m_isFirst && !m_broker ->getFirstRef( ) ) 
				co_await writeCommand( *m_socket, Command::Pair::NoPair );
			else
				m_isReady = true;
		} while( !m_isReady );
		co_await writeCommand( *m_socket, Command::Pair::Ready_ );

		socket_t socket = Pair::create( 
			m_socket, m_broker ->getFirstRef( ), m_broker, m_config )
			->start( );
		if ( m_isFirst ) 
			m_broker ->getFirstRef( ) = socket;
		else 
			m_broker ->getSecondRef( ) = socket;
		co_await socket ->async_wait( tcp::socket::wait_error, c_tuple );
	}
	auto start(Private) {
		co_spawn( m_socket ->get_executor( )
				, [self = shared_from_this( )] { 
					return self ->negotiate( );
				}
				, c_detached 
			);
		return weak_from_this( );
	}

public:
	typedef std::weak_ptr< Negotiator > negotiator_t;
	Negotiator(Private, Context const& context) :
		m_socket( context.socket )
		, m_broker( context.broker )
		, m_config( context.config )
		, m_isFirst( context.first )
	{}
	bool isReady() {
		return m_isReady;
	}
	static auto create(Context const& context) {
		return std::make_shared< Negotiator >( Private( ), context ) ->start( { } );
	}
};
} // namespace syscross::BenchP2p::Behavior::SinglePair
