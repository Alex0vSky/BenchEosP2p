// BenchP2p.cpp - entry point // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
#include "stdafx.h"
#if defined( BOOST_ASIO_ENABLE_HANDLER_TRACKING )
#	if defined( WIN32 ) 
#		define use_awaitable \
  boost::asio::use_awaitable_t(__FILE__, __LINE__, __FUNCTION__)
#	else
#		define use_awaitable \
  boost::asio::use_awaitable_t(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#	endif
#	define c_tuple \
  as_tuple( use_awaitable )
#endif
#include "Main.h"

int main(int argc, char *argv[]) {
#ifdef _DEBUG
    HeapSetInformation( NULL, HeapEnableTerminationOnCorruption, NULL, NULL );
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc( 155 );
	new char[]{ "Goobay!" };
#endif
	(void)argv;
	syscross::BenchP2p::Main main;
	try {
		main.run( argc, argv );
	} 
	catch (std::exception &exc) {
		LOG( "[E] std::exception::what = '%s'", exc.what( ) );
		LOG( "[~] press [Enter] to exit" );
		getchar( );
	}
	// TODO(alex): unprivate, boost\asio\detail\handler_tracking.hpp
//#if defined( BOOST_ASIO_ENABLE_HANDLER_TRACKING ) // memory leak
//	delete boost::asio::detail::handler_tracking::get_state( ) ->current_completion_;
//	delete boost::asio::detail::handler_tracking::get_state( ) ->current_location_;
//#endif
	return 0;
}
