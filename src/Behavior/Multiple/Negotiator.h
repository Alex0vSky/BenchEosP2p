// src\Behavior\Multiple\Negotiator.h - negotiator to create server object
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Behavior::Multiple { 
class Negotiator final : public std::enable_shared_from_this< Negotiator >, public Net::Communicator {
	struct Private{};
	bool m_isReady = false;
	tcp::socket m_socket;
	IBrokerMulti *m_broker = nullptr;
	config_t m_config;

	awaitable serverDetection() {
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
		tcp::socket *server = Server::create( std::move( m_socket ), m_broker, m_config )
			->start( );
		m_broker ->setServer( server );
		// waiting for the server to complete. OR void wait_handler(const boost::system::error_code& error)
		boost::system::error_code error;
		std::tie( error ) = co_await server ->async_wait( 
			tcp::socket::wait_error, c_tuple );
		LOG( "[~] end negotiator %d '%s'", error.value( ), error.message( ).c_str( ) );
	}

public:
	typedef std::weak_ptr< Negotiator > serverDetector_t;
	Negotiator(tcp::socket &&socket, IBrokerMulti *broker, config_t config) :
		m_socket( std::move( socket ) )
		, m_broker( broker )
		, m_config( config )
	{}
	~Negotiator() 
	{}
	void start(Private) {
		co_spawn( m_socket.get_executor( )
				, [self = shared_from_this( )] { 
					return self ->serverDetection( );
				}
				, c_detached 
			);
	}
	bool isServerReady() {
		return m_isReady;
	}
	static serverDetector_t create(tcp::socket &&socket, IBrokerMulti *broker, config_t config) {
		auto detector = std::make_shared< Negotiator >( std::move( socket ), broker, config );
		detector ->start( { } );
		return detector;
	}
};
} // namespace syscross::BenchP2p::Behavior::Multiple
