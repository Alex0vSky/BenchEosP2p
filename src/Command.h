// src\Command.h - answers, @insp https://toby-allsopp.github.io/2017/04/22/coroutines-reference-params.html
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p { 
struct Command {
	typedef std::array< char, 9 > type;
	// Simple pairs
	struct Pair {
		static constexpr type NoPair = { "NoPair\r\n" };
		static constexpr type NoMore = { "NoMore\r\n" };
	};
	// Multi clients
	struct Multi {
		static constexpr type ImServ = { "ImServ\r\n" };
		static constexpr type ImClie = { "ImClie\r\n" };
		static constexpr type Accept = { "Accept\r\n" };
		static constexpr type Denied = { "Denied\r\n" };
		static constexpr type WaitCl = { "WaitCl\r\n" };
	};
};
bool operator == (const data_t data, Command::type const& cmd) {
	return !memcmp( data, &cmd, cmd.size( ) - 1 );
}
} // namespace syscross::BenchP2p
