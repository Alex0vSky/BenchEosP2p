// src\Ui\CmdLine\Constants.h - constant values
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Ui::CmdLine { 
struct Constants {
static auto getVersion() {
	static const auto value = ( 
"BenchEosP2p version " A0S_STRING_VERSION "\n"
		);
	return value;
}

static auto getCopyright() {
	static const auto value = ( A0S_Copyright "\n\n" );
	return value;
}

static auto getUsage() {
	static const auto value = ( 
"Bench Epic EOS p2p network emergency emulator\n\n"
		);
	return value;
}
};
} // namespace syscross::BenchP2p::Ui::CmdLine _
