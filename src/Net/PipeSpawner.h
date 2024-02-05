// src\Net\PipeSpawner.h - just spawn helper
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Net { 
template<typename T>
struct PipeSpawner {
	socket_t start() {
		auto that = static_cast< T* >( this );
		co_spawn( that ->getSocket( ) ->get_executor( )
				, [self = that ->shared_from_this( )] { 
					return self ->pipe( );
				}
				, c_detached 
			);
		return that ->getSocket( );
	}
};
} // namespace syscross::BenchP2p::Net
