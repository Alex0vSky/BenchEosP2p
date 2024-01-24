// src\Behavior\Multiple\Negotiator.h - negotiator to create server object
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Behavior::Multiple { 
class Negotiator {
	class ServerDetector final : public std::enable_shared_from_this< ServerDetector >, public Net::Communicator {
		struct Private{};
		bool m_isReady = false;
		tcp::socket m_socket;
		IBrokerMulti *m_broker = nullptr;
		awaitable pipe() {
			// TODO(alex): waiting time ?@insp https://stackoverflow.com/questions/17364318/asynchronously-waiting-until-a-socket-is-available-for-reading-writing-in-asio
			co_await readData( m_socket );
			if ( getError( ) ) {
				LOG( "[E] error %d '%s'", getError( ).value( ), getError( ).message( ).c_str( ) );
				co_return;
			}
			if ( Command::Multi::
ImServ
				!= getData( ) ) {
				co_await writeCommand( m_socket, Command::Multi::Denied );
				co_return;
			}
			co_await writeCommand( m_socket, Command::Multi::Accept );
			m_isReady = true;
			tcp::socket *server = Server::create( std::move( m_socket ), m_broker )
				->start( );
			m_broker ->setServer( server );
			// OR void wait_handler(const boost::system::error_code& error)
			boost::system::error_code error;
			std::tie( error ) = co_await server ->async_wait( 
				tcp::socket::wait_error, c_tuple );
			LOG( "[~] end negotiation %d '%s'", error.value( ), error.message( ).c_str( ) );
		}

	public:
		ServerDetector(tcp::socket &&socket, IBrokerMulti *broker) :
			m_socket( std::move( socket ) )
			, m_broker( broker )
		{}
		~ServerDetector() 
		{}
		void start(Private) {
			co_spawn( m_socket.get_executor( )
					, [self = shared_from_this( )] { 
						return self ->pipe( );
					}
					, c_detached 
				);
		}
		bool isServerReady() {
			return m_isReady;
		}
	};

public:
	typedef std::weak_ptr< Negotiator::ServerDetector > serverDetector_t;

	static serverDetector_t create(tcp::socket &&socket, IBrokerMulti *broker) {
		auto detector = std::make_shared< ServerDetector >( std::move( socket ), broker );
		detector ->start( { } );
		return detector;
	}
};
} // namespace syscross::BenchP2p::Behavior::Multiple
