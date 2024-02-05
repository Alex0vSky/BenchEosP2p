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

	Channels &m_chanels;
	struct {
		Channels &m_chanels;
		awaitable gotFirst() {
			co_await m_chanels.m_ch1.async_receive( use_awaitable );
		}
		awaitable allowFirst() {
			co_await m_chanels.m_ch2.async_receive( use_awaitable );
		}
		awaitable allowPair() {
			co_await m_chanels.m_ch3.async_receive( use_awaitable );
		}
	} eventWait{ m_chanels };
	struct {
		Channels &m_chanels;
		awaitable gotFirst() {
			co_await m_chanels.m_ch1.async_send( boost::system::error_code{ }, 1, use_awaitable );
		}
		awaitable allowFirst() {
			co_await m_chanels.m_ch2.async_send( boost::system::error_code{ }, 1, use_awaitable );
		}
		awaitable allowPair() {
			co_await m_chanels.m_ch3.async_send( boost::system::error_code{ }, 1, use_awaitable );
		}
	} eventFire{ m_chanels };

	awaitable sleep() {
		boost::posix_time::milliseconds duration{ 500 };
		boost::asio::deadline_timer timer( co_await boost::asio::this_coro::executor, duration );
		co_await timer.async_wait( use_awaitable );
	}

	awaitable negotiate() {
		while( true ) {
			Command::type command = co_await readCommand( *m_socket );
			if ( Command::Pair::Hello_ != command ) {
				co_await writeCommand( *m_socket, Command::Pair::Wrong_ );
				co_return;
			}

			// if first, and not pair
			if ( m_isFirst && !m_broker ->isPaired( ) ) {
				co_await writeCommand( *m_socket, Command::Pair::NoPair );
				continue;
			}

			socket_t socket;
			if ( m_isFirst ) {
				socket = Behavior::Pair::create( 
					m_socket, nullptr, m_broker, m_config )
					->start( );
				m_broker ->getFirstRef( ) = socket;
				co_await eventFire.gotFirst( );
				co_await sleep( ); // tmp, race check
				co_await eventWait.allowPair( );
				co_await sleep( ); // tmp, race check
				co_await writeCommand( *socket, Command::Pair::Ready_ );
			} else {
				m_broker ->isPaired( ) = true;
				co_await eventWait.gotFirst( );
				socket = Behavior::Pair::create( 
					m_socket, m_broker ->getFirstRef( ), m_broker, m_config )
					->start( );
				m_broker ->getSecondRef( ) = socket;
				co_await sleep( ); // tmp, race check
				co_await writeCommand( *socket, Command::Pair::Ready_ );
				co_await eventFire.allowPair( );
				__nop( );
			}
			
			m_isReady = true;
			co_await socket ->async_wait( tcp::socket::wait_error, c_tuple );
			m_broker ->isPaired( ) = false;
			break;
		}
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
		, m_chanels( context.chanels )
	{}
	~Negotiator()
	{}
	bool isReady() {
		return m_isReady;
	}
	static auto create(Context const& context) {
		return std::make_shared< Negotiator >( Private( ), context ) ->start( { } );
	}
};
} // namespace syscross::BenchP2p::Behavior::SinglePair
