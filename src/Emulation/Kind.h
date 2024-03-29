// src\Emulation\Kind.h - kind of emulation
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Emulation { 
enum class Kind {
	begin, 
	ordinary, 
	lossOfEveryN, 
	longPing, 
	bandwidth, 
	finish, 
};
enum class Kind2 {
	EveryN, 
	// Affect after first 10 seconds
	Percent, 
	// Like `sin()` for example
	ByFunction, 
};
} // namespace syscross::BenchP2p::Emulation
