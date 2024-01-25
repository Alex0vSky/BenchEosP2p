// BenchP2p.cpp - entry point // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
#include "stdafx.h"
#if defined( BOOST_ASIO_ENABLE_HANDLER_TRACKING )
#	if defined( _MSC_VER ) 
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

int wmain(int argc, wchar_t *argv[]) {
#ifdef _DEBUG
    HeapSetInformation( NULL, HeapEnableTerminationOnCorruption, NULL, NULL );
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc( 155 );
	new char[]{ "Goobay!" };
#endif
	// for `boost::system::error_code::message` language
	setlocale( 0, "" );
#if defined( _MSC_VER ) 
	// TODO(alex): Neutral language @insp https://www.curlybrace.com/words/2008/06/10/setthreadlocale-and-setthreaduilanguage-for-localization-on-windows-xp-and-vista/
#endif
	syscross::BenchP2p::Main main;
	int returnValue = ERROR_UNHANDLED_EXCEPTION;
	try {
		returnValue = main.run( argc, argv );
	} 
	catch (std::exception &exc) {
		LOG( "[E] std::exception::what = '%s'\npress [Enter] to exit", exc.what( ) );
		getchar( );
	}
	// TODO(alex): unprivate, boost\asio\detail\handler_tracking.hpp
//#if defined( BOOST_ASIO_ENABLE_HANDLER_TRACKING ) // memory leak
//	delete boost::asio::detail::handler_tracking::get_state( ) ->current_completion_;
//	delete boost::asio::detail::handler_tracking::get_state( ) ->current_location_;
//#endif
	return returnValue;
}
