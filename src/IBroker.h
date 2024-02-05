// src\IBroker.h - broker interface
#pragma once // Copyright 2024 Alex0vSky (https://github.com/Alex0vSky)
namespace syscross::BenchP2p { 
struct IBrokerPair {
	virtual ~IBrokerPair() {}
	virtual socket_t getPair() const = 0;
	virtual void drop() = 0;
	virtual socket_t &getFirstRef() = 0;
	virtual socket_t &getSecondRef() = 0;
};
struct IBrokerMulti {
	virtual ~IBrokerMulti() {}
	virtual peers_t *getClients() = 0;
	virtual void remove(tcp::socket *) = 0;
	virtual void drop() = 0;
	virtual void setServer(tcp::socket *) = 0;
};
} // namespace syscross::BenchP2p
