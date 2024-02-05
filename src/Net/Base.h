// src\Net\Base.h - stopable and pipeable
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Net { 
class Base : public Communicator {
	socket_t m_socket;
	Emulation::Emulator m_emergencyEmulator;

	virtual peers_t *getPeers(cref_data_t, Command::type *) = 0;

protected:
	template<typename T> friend struct PipeSpawner;
	socket_t getSocket() {
		return m_socket;
	}
	awaitable pipe() {
		bool isError = false;
		while ( !isError ) {
			std::size_t n = co_await readData( *m_socket );
			if ( getError( ) )
				break;
			Command::type command = { };
			peers_t *peers = getPeers( getData( ), &command );
			if ( Command::type{ } != command ) 
				co_await writeCommand( *m_socket, command );
			if ( !peers ->size( ) ) 
				continue;
			if ( co_await m_emergencyEmulator.handle( n ) )
				continue;

			for ( auto &peer : *peers ) 
				if ( isError = co_await writeData( peer.get( ), n ) ) 
					break;
		}
		LOG( "[~] stop by %d", getError( ).value( ) );
		m_socket ->close( );
	}
	Base(socket_t socket, config_t config) : 
		m_socket( socket )
		, m_emergencyEmulator( config, m_socket ->get_executor( ) )
	{}

public:
	virtual ~Base() {}
};
} // namespace syscross::BenchP2p::Net
