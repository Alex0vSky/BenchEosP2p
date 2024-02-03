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
#include "Ui/CmdLine/StringConvert.h"
#include "Ui/CmdLine/ArgparseImpl.h"

namespace syscross::BenchP2p { 
struct Main {
	int run(int argc, wchar_t *argv[]) {
		Ui::CmdLine::ArgparseImpl cmdLiner( argc, argv );
		Emulation::Config config{ .longPingDuration_milli = 1000 };
		u_short port = 55555;
		if ( !cmdLiner.parse( &config, &port ) )
			return ERROR_BAD_ARGUMENTS;

	    boost::asio::io_context io_context( 1 );
		SignalSet signalSet( io_context );
		signalSet.ordinary( );
		auto acceptor = tcp::acceptor( io_context, { tcp::v4( ), port } );
		co_spawn( 
				io_context
				, BrokerPair::listener( std::move( acceptor ), config )
				//, BrokerMultiple::listener( std::move( acceptor ), config )
				, c_detached
			);
		io_context.run( );
		return 0;
	}
};
} // namespace syscross::BenchP2p
