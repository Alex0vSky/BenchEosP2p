// src\Emulation\Kind.h - kind of emulation
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Emulation { 
enum class Kind {
	EvenN, 
	// Affect after first 10 seconds
	Percent, 
	// Like `sin()` for example
	ByFunction, 
};
} // namespace syscross::BenchP2p::Emulation
