// src\Ui\CmdLine\ArgparseImpl.h - command line working, 'argparse' declaration
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Ui::CmdLine {
class ArgparseImpl {
	StringConvert::ArgvChar m_ArgvChar;

 public:
	ArgparseImpl(int argc, wchar_t *argv[]) : m_ArgvChar( argc, argv )
    {}

	bool parse(Emulation::Config *config, u_short *port);
};
} // namespace syscross::BenchP2p::Ui::CmdLine
