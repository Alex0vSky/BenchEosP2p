// src\Net\Base.h - stopable and pipeable
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Net { 
class Base : public Communicator {
	Emulation::Emulator m_emulation;
	tcp::socket m_socket;
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
			Command::type command;
			peers_t *peers = getPeers( getData( ), &command );
			if ( !peers ->size( ) ) {
				co_await writeCommand( m_socket, command );
				continue;
			}
			if ( m_emulation.handle( n ) )
				continue;

			for ( auto &peer : *peers ) {
				if ( !co_await writeData( *peer, n ) ) {
					break;
				}
			}
		}
		LOG( "[E] error %d '%s'", getError( ).value( ), getError( ).message( ).c_str( ) );
	    m_socket.close( );
	}
	Base(tcp::socket &&socket) : 
		m_socket( std::move( socket ) )
	{}

public:
	virtual ~Base() {}
};
} // namespace syscross::BenchP2p::Net
