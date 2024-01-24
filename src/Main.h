// src\Main.h - code main class, for unittests code coverage
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
#include "Version.h"
#include "Log.h"
#include "aliases.h"
#include "SignalSet.h"
#include "IBroker.h"
#include "Command.h"
#include "Emulation/Kind.h"
#include "Emulation/Config.h"
#include "Emulation/Statistics.h"
#include "Emulation/Emulator.h"
#include "Net/Communicator.h"
#include "Net/Base.h"
#include "Net/PipeSpawner.h"
#include "Behavior/Pair.h"
#include "Behavior/Multiple/Client.h"
#include "Behavior/Multiple/Server.h"
#include "Behavior/Multiple/Negotiator.h"
#include "BrokerPair.h"
#include "BrokerMultiple.h"

namespace syscross::BenchP2p { 
struct Main {
	void run(int argc, char *argv[]) {
	    boost::asio::io_context io_context( 1 );
		SignalSet signalSet( io_context );
		signalSet.ordinary( );
		u_short port = 55555;
		if ( argc > 1 )
			port = static_cast< u_short >( std::atoi( argv[ 1 ] ) );
		auto acceptor = tcp::acceptor( io_context, { tcp::v4( ), port } );
		co_spawn( 
				io_context
				//, BrokerPair::listener( std::move( acceptor ) )
				, BrokerMultiple::listener( std::move( acceptor ) )
				, c_detached
			);
		io_context.run( );
	}
};
} // namespace syscross::BenchP2p
