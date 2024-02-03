// src\Net\Base.h - stopable and pipeable
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Net { 
class Base : public Communicator {
	tcp::socket m_socket;
	Emulation::Emulator m_emergencyEmulator;

	virtual peers_t *getPeers(cref_data_t, Command::type *) = 0;

protected:
	template<typename T> friend struct PipeSpawner;
	tcp::socket *getSocket() {
		return &m_socket;
	}
	awaitable pipe() {
		while ( true ) {
			std::size_t n = co_await readData( m_socket );
			if ( getError( ) )
				break;
			Command::type command = { };
			peers_t *peers = getPeers( getData( ), &command );
			if ( Command::type{ } != command ) 
				co_await writeCommand( m_socket, command );
			if ( !peers ->size( ) ) 
				continue;
			if ( co_await m_emergencyEmulator.handle( n ) )
				continue;

			for ( auto &peer : *peers ) 
				if ( !co_await writeData( *peer, n ) ) 
					break;
		}
		LOG( "[E] error %d '%s'", getError( ).value( ), getError( ).message( ).c_str( ) );
	    m_socket.close( );
	}
	Base(tcp::socket &&socket, config_t config) : 
		m_socket( std::move( socket ) )
		, m_emergencyEmulator( config, m_socket.get_executor( ) )
	{}

public:
	virtual ~Base() {}
};
} // namespace syscross::BenchP2p::Net
