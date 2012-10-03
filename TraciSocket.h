/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

/************************************************************************
 * Modification history :
 * 10/03/2012 - Rudhir
 ************************************************************************/

#ifndef __TRACI_CLIENT_SOCKET_H
#define __TRACI_CLIENT_SOCKET_H

#include "TraciStorage.h"
#include <string>
#include <map>
#include <vector>
#include <list>
#include <deque>
#include <iostream>
#include <cstddef>

struct in_addr;

class TraciSocketException: public std::exception {
private:
	std::string excpMsg;

public:
	TraciSocketException(std::string msg) throw () {
		excpMsg = msg;
	}

#if 0
	virtual const char* getExcpMsg() const throw () {
		return excpMsg.c_str();
	}
#endif
	virtual const std::string getExcpMsg() const throw () {
		return excpMsg;
	}

	~TraciSocketException() throw () {
	}
};

class TraciSocket {
	friend class Response;
public:
	/// Constructor that prepare to connect to host:port
	TraciSocket(std::string host, int port);

	/// Constructor that prepare for accepting a connection on given port
	TraciSocket(int port);

	/// Destructor
	~TraciSocket();

	/// Connects to host_:port_
	void traciConnect() throw (TraciSocketException);

	/// Wait for a incoming connection to port_
	void traciAccept() throw (TraciSocketException);

	void traciSend(const std::vector<unsigned char> &buffer) throw (TraciSocketException);
	void traciSendExact(const TraciStorage &) throw (TraciSocketException);

	/// Receive up to bufSize available bytes from Socket::socket_
	std::vector<unsigned char> traciReceive(int bufSize = 2048)
			throw (TraciSocketException);
	/// Receive a complete TraCI message from Socket::socket_
	bool traciReceiveExact(TraciStorage &) throw (TraciSocketException);
	void traciClose();
	int getPort();
	void setBlocking(bool) throw (TraciSocketException);
	bool isBlocking() throw ();
	bool hasClientConnection() const;

	// If verbose, each send and received data is written to stderr
	bool verbose() {
		return verbose_;
	}
	
	void setVerbose(bool newVerbose) {
		verbose_ = newVerbose;
	}

protected:
	/// Length of the message length part of a TraCI message
	static const int lengthLen;

	/// Receive len bytes from Socket::socket_
	void receiveComplete(unsigned char * const buffer, std::size_t len) const;
	/// Receive up to len available bytes from Socket::socket_
	size_t recvAndCheck(unsigned char * const buffer, std::size_t len) const;
	/// Print label and buffer to stderr if Socket::verbose_ is set
	void printBufferOnVerbose(const std::vector<unsigned char> buffer,
			const std::string &label) const;

private:
	void init();
	void BailOnSocketError(std::string) const throw (TraciSocketException);
	int atoaddr(std::string, struct in_addr& addr);
	bool dataWaiting(int sock) const throw ();
	std::string host_;
	int port_;
	int socket_;
	int server_socket_;
	bool blocking_;
	bool verbose_;
};

#endif // __TRACI_CLIENT_SOCKET_H
