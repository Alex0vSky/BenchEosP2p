// src\IBroker.h - broker interface
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p { 
struct IBrokerPair {
	virtual ~IBrokerPair() {}
	virtual tcp::socket *getPair() const = 0;
	virtual void drop() = 0;
};
struct IBrokerMulti {
	virtual ~IBrokerMulti() {}
	virtual peers_t *getClients() const = 0;
	virtual void remove(tcp::socket *) = 0;
	virtual void drop() = 0;
};
} // namespace syscross::BenchP2p
