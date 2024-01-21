// src\Net\Base.h - stopable and piped
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Net { 
class Base : public Communicator {
	Emulation::Emulator m_emulation;
	tcp::socket m_socket;
	virtual tcp::socket *getPeer(Command::type *command) = 0;

protected:
	tcp::socket *getSocket() {
		return &m_socket;
	}
	awaitable pipe() {
		while ( true ) {
			std::size_t n = co_await readData( m_socket );
			if ( getError( ) )
				break;
			Command::type command;
			tcp::socket *peer = nullptr;
			if ( !( peer = getPeer( &command ) ) ) {
				co_await writeCommand( m_socket, command );
				continue;
			}
			if ( m_emulation.handle( n ) )
				continue;
			if ( !co_await writeData( *peer, n ) )
				break;
		}
		LOG( "[E] error %d", getError( ).value( ) );
	    m_socket.close( );
	}
	Base(tcp::socket &&socket) : 
		m_socket( std::move( socket ) )
	{}

public:
	virtual ~Base() {}
};
} // namespace syscross::BenchP2p::Net
