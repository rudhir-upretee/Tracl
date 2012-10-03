/************************************************************************
 * Modification history :
 * 10/03/2012 - Rudhir
 ************************************************************************/

#include "SocketUnitTests.h"
#include "TraciStorage.h"
#include "TraciSocket.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string.h>
#include <cstring>
#include <limits.h>
#include <float.h>
#include <cassert>

#define ASSERT assert
#define ASSERT_EQUALS(a, b) assert(a==b)

#define ADDRINUSE 
#define PORT 8888

#define TEST_CHAR 'a'
#define TEST_BYTE 100
#define TEST_STRING "Test-String"
#define TEST_SHORT 1600
#define TEST_INT 32000
#define TEST_FLOAT 10.4
#define TEST_DOUBLE 10.4568

SocketUnitTests::SocketUnitTests()
{
	serverSocket = new TraciSocket(PORT);
	clientSocket = new TraciSocket("localhost", PORT);
	currentMode = undefined;
}

void SocketUnitTests::openConnection()
{
	try
		{
		serverSocket->setBlocking(false);
		serverSocket->traciAccept();
		currentMode = server;
		}
	catch(TraciSocketException &e)
		{
		cout << "ExcpMsg = " << e.getExcpMsg() << endl;
		if (e.getExcpMsg() ==
			"TraciSocket::traciAccept Unable to create listening socket: Address already in use")
			{
			currentMode = client;
			}
		else
			{
			cout << "Server socket accept() failed!" << endl;
			exit(-1);
			}
		}

	if (currentMode == server)
		{
		serverSocket->setBlocking(true);
		cout << "Listening on port " << PORT
			 << ". Please start another instance of this program on host.\n";
		serverSocket->traciAccept();
		}

	if (currentMode == client)
		{
		try
			{
			clientSocket->traciConnect();
			}
		catch (TraciSocketException &e)
			{
			cout << "Client socket connect() failed!" << endl;
			exit(-1);
			}
		}
}

void SocketUnitTests::closeConnection()
{
	try
		{
		if (currentMode == client)
			clientSocket->traciClose();
		}
	catch (TraciSocketException &e)
		{
		cout << "Client socket close() failed!" << endl;
		return;
		}

	try
		{
		if (currentMode == server)
			serverSocket->traciClose();
		}
	catch (TraciSocketException &e)
		{
		cout << "Server socket close() failed!" << endl;
		return;
		}
}

void SocketUnitTests::receiveStorage()
{
	if (currentMode == client)
		return;

	TraciStorage receivedMessage;

	try
		{
		serverSocket->traciReceiveExact(receivedMessage);

		ASSERT_EQUALS( static_cast<int>(TEST_CHAR),
				       static_cast<int>(receivedMessage.readChar()) );
		ASSERT_EQUALS( TEST_STRING, receivedMessage.readString() );
		ASSERT_EQUALS( TEST_SHORT, receivedMessage.readShort() );
		ASSERT_EQUALS( TEST_INT, receivedMessage.readInt() );
		ASSERT_EQUALS( (float)TEST_FLOAT, receivedMessage.readFloat() );
		ASSERT_EQUALS( (double)TEST_DOUBLE, receivedMessage.readDouble() );
		cout << "Server socket receiveStorage() passed!" << endl;
		}
	catch (TraciSocketException &e)
		{
		cout << "Server socket receiveStorage() failed!" << endl;
		return;
		}
}

void SocketUnitTests::sendStorage()
{
	if (currentMode == server)
		return;

	TraciStorage sendMessage;

	try
		{
		sendMessage.writeChar( TEST_CHAR );
		sendMessage.writeString( TEST_STRING );
		sendMessage.writeShort( TEST_SHORT );
		sendMessage.writeInt( TEST_INT );
		sendMessage.writeFloat( TEST_FLOAT );
		sendMessage.writeDouble( TEST_DOUBLE );

		clientSocket->traciSendExact(sendMessage);
		cout << "Client socket sendStorage() passed!" << endl;
		}
	catch (TraciSocketException &e)
		{
		cout << "Client socket sendStorage() failed!" << endl;
		return;
		}

}





