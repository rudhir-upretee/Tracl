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

#include "TraciSocket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const int TraciSocket::lengthLen = 4;

TraciSocket::TraciSocket(string host, int port) {
	
	host_ = host;
	port_ = port;
	socket_ = -1;
	server_socket_ = -1;
	blocking_ = true;
	verbose_ = false;
	init();
}

TraciSocket::TraciSocket(int port) {
	
	host_ = "";
	port_ = port;
	socket_ = -1;
	server_socket_ = -1;
	blocking_ = true;
	verbose_ = false;
	init();
}

void TraciSocket::init() {
	// Nothing to do
}

TraciSocket::~TraciSocket() {
	// Close first an existing client connection ...
	traciClose();

	// ... then the server socket
	if (server_socket_ >= 0) {
		close (server_socket_);
		server_socket_ = -1;
	}

}

void TraciSocket::BailOnSocketError(string context) const
		throw (TraciSocketException) {
	string msg = strerror(errno);
	cout << "BailErr = " << context << ": " << msg << endl;
	throw TraciSocketException(context + ": " + msg);
}

int TraciSocket::getPort() {
	return port_;
}

bool TraciSocket::dataWaiting(int sock) const throw () {
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(sock, &fds);

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	int r = select(sock + 1, &fds, NULL, NULL, &tv);

	if (r < 0)
		BailOnSocketError("TraciSocket::dataWaiting @ select");

	if (FD_ISSET(sock, &fds))
		return true;
	else
		return false;
}

int TraciSocket::atoaddr(string address, struct in_addr& addr) {
	struct hostent* host;
	struct in_addr saddr;

	// First try nnn.nnn.nnn.nnn form
	saddr.s_addr = inet_addr(address.c_str());
	if (saddr.s_addr != static_cast<unsigned int>(-1)) {
		addr = saddr;
		return 0;
	}

	host = gethostbyname(address.c_str());
	if (host) {
		addr = *((struct in_addr*) host->h_addr_list[0]);
		return 0;
	}

	return -1;
}

void TraciSocket::traciAccept() throw (TraciSocketException) {
	if (socket_ >= 0)
		return;

	struct sockaddr_in client_addr;
	socklen_t addrlen = sizeof(client_addr);

	if (server_socket_ < 0) {
		struct sockaddr_in self;

		//Create the server socket
		server_socket_ = static_cast<int>(socket(AF_INET, SOCK_STREAM, 0));
		if (server_socket_ < 0)
			BailOnSocketError("TraciSocket::traciAccept @ socket");

		//"Address already in use" error protection
		{
			int reuseaddr = 1;

			setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &reuseaddr,
					sizeof(reuseaddr));
		}

		// Initialize address/port structure
		memset(&self, 0, sizeof(self));
		self.sin_family = AF_INET;
		self.sin_port = htons(port_);
		self.sin_addr.s_addr = htonl(INADDR_ANY);

		cout << "memset success" << endl;
		// Assign a port number to the socket
		if (bind(server_socket_, (struct sockaddr*) &self, sizeof(self)) != 0)
			BailOnSocketError(
					"TraciSocket::traciAccept Unable to create listening socket");
		cout << "bind success" << endl;

		// Make it a "listening socket"
		if (listen(server_socket_, 10) == -1)
			BailOnSocketError(
					"traciSocket::traciAccept Unable to listen on server socket");
		cout << "listen success" << endl;

		// Make the newly created socket blocking or not
		setBlocking (blocking_);
	}

	socket_ = static_cast<int>(accept(server_socket_,
			                         (struct sockaddr*) &client_addr, &addrlen));

	if (socket_ >= 0) {
		int x = 1;
		setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (const char*) &x,
				sizeof(x));
	}
}

void TraciSocket::setBlocking(bool blocking) throw (TraciSocketException) {
	blocking_ = blocking;

	if (server_socket_ > 0) {
		long arg = fcntl(server_socket_, F_GETFL, NULL);
		if (blocking_) {
			arg &= ~O_NONBLOCK;
		} else {
			arg |= O_NONBLOCK;
		}
		fcntl(server_socket_, F_SETFL, arg);
	}

}

void TraciSocket::traciConnect() throw (TraciSocketException) {
	in_addr addr;
	if (atoaddr(host_.c_str(), addr) != 0)
		BailOnSocketError("TraciSocket::traciConnect @ Invalid network address");

	sockaddr_in address;
	memset((char*) &address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port_);
	address.sin_addr.s_addr = addr.s_addr;

	socket_ = static_cast<int>(socket(PF_INET, SOCK_STREAM, 0));
	if (socket_ < 0)
		BailOnSocketError("TraciSocket::traciConnect @ socket");

	if (connect(socket_, (sockaddr const*) &address, sizeof(address)) < 0)
		BailOnSocketError("TraciSocket::traciConnect @ connect");

	if (socket_ >= 0) {
		int x = 1;
		setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (const char*) &x,
				sizeof(x));
	}

}

void TraciSocket::traciClose() {
	// Close client-connection 
	if (socket_ >= 0) {
		close (socket_);
		socket_ = -1;
	}
}

void TraciSocket::traciSend(const std::vector<unsigned char> &buffer)
		throw (TraciSocketException) {
	if (socket_ < 0)
		return;

	printBufferOnVerbose(buffer, "Send");

	size_t numbytes = buffer.size();
	unsigned char const *bufPtr = &buffer[0];
	while (numbytes > 0) {
		int bytesSent = send(socket_, bufPtr, numbytes, 0);
		if (bytesSent < 0)
			BailOnSocketError("TraciSocket::traciSend send failed");

		numbytes -= bytesSent;
		bufPtr += bytesSent;
	}
}

void TraciSocket::traciSendExact(const TraciStorage &b) throw (TraciSocketException) {
	int length = static_cast<int>(b.size());
	TraciStorage length_storage;
	length_storage.writeInt(lengthLen + length);

	// Sending length_storage and b independently would probably be possible and
	// avoid some copying here, but both parts would have to go through the
	// TCP/IP stack on their own which probably would cost more performance.
	vector<unsigned char> msg;
	msg.insert(msg.end(), length_storage.begin(), length_storage.end());
	msg.insert(msg.end(), b.begin(), b.end());
	traciSend(msg);
}

size_t TraciSocket::recvAndCheck(unsigned char * const buffer,
		                         std::size_t len) const {
	const int bytesReceived = static_cast<int>(recv(socket_, buffer, len, 0));
	if (bytesReceived == 0)
		throw TraciSocketException(
				"TraciSocket::recvAndCheck @ recv: peer shutdown");
	if (bytesReceived < 0)
		BailOnSocketError("TraciSocket::recvAndCheck @ recv");

	return static_cast<size_t>(bytesReceived);
}

void TraciSocket::receiveComplete(unsigned char * buffer, size_t len) const {
	while (len > 0) {
		const size_t bytesReceived = recvAndCheck(buffer, len);
		len -= bytesReceived;
		buffer += bytesReceived;
	}
}

vector<unsigned char> TraciSocket::traciReceive(int bufSize) throw (TraciSocketException) {

	vector<unsigned char> buffer;

	if (socket_ < 0)
		traciConnect();

	if (!dataWaiting(socket_))
		return buffer;

	buffer.resize(bufSize);
	const size_t bytesReceived = recvAndCheck(&buffer[0], bufSize);

	buffer.resize(bytesReceived);

	printBufferOnVerbose(buffer, "Rcvd");

	return buffer;
}

bool TraciSocket::traciReceiveExact(TraciStorage &msg) throw (TraciSocketException) {
	// buffer for received bytes
	// According to the C++ standard elements of a std::vector are stored
	// contiguously. Explicitly &buffer[n] == &buffer[0] + n for 0 <= n < buffer.size().
	vector<unsigned char> buffer(lengthLen);

	// receive length of TraCI message
	receiveComplete(&buffer[0], lengthLen);
	TraciStorage length_storage(&buffer[0], lengthLen);
	const int totalLen = length_storage.readInt();
	assert(totalLen > lengthLen);

	// extent buffer
	buffer.resize(totalLen);

	// receive remaining TraCI message
	receiveComplete(&buffer[lengthLen], totalLen - lengthLen);

	// copy message content into passed Storage
	msg.reset();
	msg.writePacket(&buffer[lengthLen], totalLen - lengthLen);

	printBufferOnVerbose(buffer, "Rcvd Storage with");

	return true;
}

bool TraciSocket::hasClientConnection() const {
	return socket_ >= 0;
}

bool TraciSocket::isBlocking() throw () {
	return blocking_;
}

void TraciSocket::printBufferOnVerbose(const std::vector<unsigned char> buffer,
		const std::string &label) const {
	if (verbose_) {
		cerr << label << " " << buffer.size() << " bytes via traciSocket: [";
		// cache end iterator for performance
		const vector<unsigned char>::const_iterator end = buffer.end();
		for (vector<unsigned char>::const_iterator it = buffer.begin();
				end != it; ++it)
			cerr << " " << static_cast<int>(*it) << " ";
		cerr << "]" << endl;
	}
}
