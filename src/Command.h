// src\Command.h - answers, @insp https://toby-allsopp.github.io/2017/04/22/coroutines-reference-params.html
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p { 
struct Command {
	typedef std::array< char, 8 > type;
	static constexpr type NoPair = { "NoPair\n" };
	static constexpr type NoMore = { "NoMore\n" };
};
} // namespace syscross::BenchP2p
