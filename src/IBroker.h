// src\IBroker.h - broker interface
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p { 
struct IBroker {
	virtual ~IBroker() {}
	virtual tcp::socket *getPair() const = 0;
};
} // namespace syscross::BenchP2p
