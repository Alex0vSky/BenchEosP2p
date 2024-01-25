// src\Emulation\Config.h - config of emulation
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p::Emulation { 
struct Config {
	std::optional< uint32_t > lossOfEveryN;
	std::optional< uint32_t > longPingDuration_milli;
	// TODO(alex): makeme
};
} // namespace syscross::BenchP2p::Emulation
