// src\Ui\CmdLine\ArgparseImpl.cpp - 'argparse' implementation, posted here to speed up compilation
#include "stdafx.h"
#pragma warning( push )
#pragma warning( disable: 4244 )
#pragma warning( disable: 4101 )
#include <argparse.hpp>
#pragma warning( pop )
#include "Version.h"
#include "Log.h"
#include "aliases.h"
#include "Emulation/Config.h"
#include "Ui/CmdLine/Constants.h"
#include "Ui/CmdLine/StringConvert.h"
#include "Ui/CmdLine/ArgparseImpl.h"

namespace syscross::BenchP2p::Ui::CmdLine {
namespace detail_ {
	class morrisfranken_argparse : public argparse::Args {
		void welcome() override {
			std::cout << Constants::getUsage( );
		}
	};

	struct MyArgs : public morrisfranken_argparse {
		u_short
			&arg_port
												= kwarg(
			"port"
, "port number to p2p connection"
"\n\t\t\t\t from 1 to 65535\n\t\t\t\t"
)
.set_default( 55555 );

		std::optional< uint32_t >
			&arg_lossOfEveryN
												= kwarg(
			"lossOfEveryN"
, "loss Of Every N packet"
"\n\t\t\t\t from 2 to 256\n\t\t\t\t"
);

		std::optional< uint32_t >
			&arg_longPingDuration_milli
												= kwarg(
			"longPing"
, "long ping duration in milliseconds"
"\n\t\t\t\t from 1 to 10000\n\t\t\t\t"
);

		std::optional< uint32_t >
			&arg_bandwidthInKBytes
												= kwarg(
			"bandwidth"
, "Channel width limitation in kilobytes per second"
"\n\t\t\t\t not zero\n\t\t\t\t"
);

		bool &arg_version						= flag(
			"version"
, "print version"
);
	};
} // namespace detail_

bool ArgparseImpl::parse(Emulation::Config *config, u_short *port) {
	int argc = m_ArgvChar.argc( );
	// Get parsed command line arguments
	detail_::MyArgs parsed = argparse::parse< detail_::MyArgs >( argc, m_ArgvChar.argv( ) );
	if ( 1 == argc ) {
	    std::cout << Constants::getVersion( );
		std::cout << Constants::getCopyright( );
		parsed.help( );
		return false;
	}
	if ( parsed.arg_version ) {
	    std::cout << Constants::getVersion( );
		::exit( ERROR_SUCCESS );
	}
	*port = parsed.arg_port;
	config ->lossOfEveryN = parsed.arg_lossOfEveryN;
	config ->longPingDuration_milli = parsed.arg_longPingDuration_milli;
	config ->bandwidthInKBytes = parsed.arg_bandwidthInKBytes;
	return true;
}
} // namespace syscross::BenchP2p::Ui::CmdLine
