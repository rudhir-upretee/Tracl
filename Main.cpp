//============================================================================
// Name        : Main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

/************************************************************************
 * Modification history :
 * 10/03/2012 - Rudhir
 ************************************************************************/

#include <iostream>
#include "StorageUnitTests.h"
#include "SocketUnitTests.h"

using namespace std;

int main() {

	cout << "--> Start Storage Test Cases <--" << endl << endl;

	StorageUnitTests sut;
	sut.testStorageChar();
	sut.testStorageByte();
	sut.testStorageString();
	sut.testStorageShort();
	sut.testStorageInt();
	sut.testStorageFloat();
	sut.testStorageDouble();
	sut.testStorageMixed();
	sut.testStorageReset();
	sut.testStorageValidPos();
	sut.testStorageLoadCharArray();
	sut.testStorageCharToInt();

	cout << endl << "--> Start Socket Test Cases <--" << endl << endl;
	SocketUnitTests sockut;
	sockut.openConnection();
	sockut.sendStorage();
	sockut.receiveStorage();
	sockut.closeConnection();

	return 0;
}
