// src\Command.h - answers, @insp https://toby-allsopp.github.io/2017/04/22/coroutines-reference-params.html
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p { 
struct Command {
	typedef std::array< char, 8 > type;

	// @insp SO constexpr-convert-a-list-of-string-views-into-a-list-of-char-arrays
private:
	template<typename=void> static constexpr auto copyString(const std::string_view s) {
		type ret{ };
		for ( std::size_t i = std::tuple_size_v< type >; i--; ) 
			ret[ i ] = s[ i ];
		return ret;
	}

public:
	// Simple pairs
	struct Pair {
		static constexpr type NoPair = copyString( "NoPair\r\n" );
		static constexpr type NoMore = copyString( "NoMore\r\n" );
		static constexpr type Hello_ = copyString( "Hello_\r\n" );
		static constexpr type Ready_ = copyString( "Ready_\r\n" );
		static constexpr type Negott = copyString( "Negott\r\n" );
		static constexpr type Wrong_ = copyString( "Wrong_\r\n" );
	};
	// Multi clients
	struct Multi {
		static constexpr type ImServ = copyString( "ImServ\r\n" );
		// TODO(alex): affectme
		static constexpr type ImClie = copyString( "ImClie\r\n" );
		static constexpr type Accept = copyString( "Accept\r\n" );
		static constexpr type Denied = copyString( "Denied\r\n" );
		static constexpr type WaitCl = copyString( "WaitCl\r\n" );
		// the server is in the process of negotiation.
		static constexpr type SrvNeg = copyString( "SrvNeg\r\n" );
	};
};
bool operator == (const data_t data, Command::type const& cmd) {
	return !memcmp( data, &cmd, cmd.size( ) );
}
bool operator == (Command::type const& cmd, const data_t data) {
	return !memcmp( data, &cmd, cmd.size( ) );
}
} // namespace syscross::BenchP2p
