/*
 * This file provides test cases for the tcpip package, that can be executed by MiniCppUnit
 *
 * Author: Axel Wegener
 */

/************************************************************************
 * Modification history :
 * 10/03/2012 - Rudhir
 ************************************************************************/

#include "TraciStorage.h"
#include "StorageUnitTests.h"
#include <math.h>
#include <cassert>

#define ASSERT assert
#define ASSERT_EQUALS(a, b) assert(a==b)

using namespace std;

void StorageUnitTests::testStorageChar()
{
	TraciStorage s1;

	s1.writeChar('\0');
	s1.writeChar('\n');
	s1.writeChar('a');

	ASSERT_EQUALS( true, s1.valid_pos() );
	ASSERT( s1.readChar() == '\0' );
	ASSERT( s1.readChar() == '\n' );
	ASSERT( s1.readChar() == 'a' );
	ASSERT_EQUALS( false, s1.valid_pos() );

	cout << "testStorageChar() PASSED" << endl;
}

void StorageUnitTests::testStorageByte()
{
	TraciStorage s1;

	s1.writeByte(0);
	s1.writeByte(-128);
	s1.writeByte(127);

	ASSERT_EQUALS( true, s1.valid_pos() );
	ASSERT_EQUALS( 0, s1.readByte() );
	ASSERT_EQUALS( -128, s1.readByte() );
	ASSERT_EQUALS( 127, s1.readByte() );
	ASSERT_EQUALS( false, s1.valid_pos() );

	cout << "testStorageByte() PASSED" << endl;
}

void StorageUnitTests::testStorageString()
{
	TraciStorage s1;

	s1.writeString("This is a TestString");
	s1.writeString("");
	s1.writeString("2. Another string\n\0");

	ASSERT_EQUALS( true, s1.valid_pos() );
	ASSERT_EQUALS( "This is a TestString", s1.readString() );
	ASSERT_EQUALS( "", s1.readString() );
	ASSERT_EQUALS( "2. Another string\n\0", s1.readString() );
	ASSERT_EQUALS( false, s1.valid_pos() );

	cout << "testStorageString() PASSED" << endl;
}

void StorageUnitTests::testStorageShort()
{
	TraciStorage s1;

	s1.writeShort(0);
	s1.writeShort(32767);
	s1.writeShort(-32768);

	ASSERT_EQUALS( true, s1.valid_pos() );
	ASSERT_EQUALS( 0, s1.readShort() );
	ASSERT_EQUALS( 32767, s1.readShort() );
	ASSERT_EQUALS( -32768, s1.readShort() );
	ASSERT_EQUALS( false, s1.valid_pos() );

	cout << "testStorageShort() PASSED" << endl;
}

void StorageUnitTests::testStorageInt()
{
	TraciStorage s1;

	s1.writeInt(0);
	s1.writeInt(-32768);
	s1.writeInt(32767);

	ASSERT_EQUALS( true, s1.valid_pos() );
	ASSERT_EQUALS( 0, s1.readInt() );
	ASSERT_EQUALS( -32768, s1.readInt() );
	ASSERT_EQUALS( 32767, s1.readInt() );
	ASSERT_EQUALS( false, s1.valid_pos() );

	cout << "testStorageInt() PASSED" << endl;
}

void StorageUnitTests::testStorageFloat()
{
	TraciStorage s1;
	float f1 = (float) 0.000002;
	float f2 = (float) 0.000001;

	s1.writeFloat(0.0);
	s1.writeFloat(f1);
	s1.writeFloat(f2);

	ASSERT_EQUALS( true, s1.valid_pos() );
	ASSERT_EQUALS( (float) 0.0, s1.readFloat() );
	ASSERT_EQUALS( f1, s1.readFloat() );
	ASSERT_EQUALS( f2, s1.readFloat() );
	ASSERT_EQUALS( false, s1.valid_pos() );

	cout << "testStorageFloat() PASSED" << endl;
}

void StorageUnitTests::testStorageDouble()
{
	TraciStorage s1;

	double d1 = 0.000002;
	double d2 = 0.000001;

	s1.writeDouble(0.0);
	s1.writeDouble(d1);
	s1.writeDouble(d2);

	ASSERT_EQUALS( true, s1.valid_pos() );
	ASSERT_EQUALS( 0.0, s1.readDouble() );
	ASSERT_EQUALS( d1, s1.readDouble() );
	ASSERT_EQUALS( d2, s1.readDouble() );
	ASSERT_EQUALS( false, s1.valid_pos() );

	cout << "testStorageDouble() PASSED" << endl;
}

void StorageUnitTests::testStorageMixed()
{
	TraciStorage s1;

	double d1 = 0.000002;

	s1.writeFloat(0.0);
	s1.writeDouble(d1);
	s1.writeString("\0");
	s1.writeShort(42);

	ASSERT_EQUALS( true, s1.valid_pos() );
	ASSERT_EQUALS( (float) 0.0, s1.readFloat() );
	ASSERT_EQUALS( d1, s1.readDouble() );
	ASSERT_EQUALS( "\0", s1.readString() );
	ASSERT_EQUALS( 42, s1.readShort() );
	ASSERT_EQUALS( false, s1.valid_pos() );

	cout << "testStorageMixed() PASSED" << endl;
}

void StorageUnitTests::testStorageReset()
{
	TraciStorage s1;

	double d1 = 0.000002;

	s1.reset();
	ASSERT_EQUALS( false, s1.valid_pos() );

	s1.writeFloat(0.0);
	s1.writeDouble(d1);
	s1.writeString("\0");
	s1.writeShort(42);

	ASSERT_EQUALS( true, s1.valid_pos() );
	s1.reset();
	ASSERT_EQUALS( false, s1.valid_pos() );

	s1.writeString("Test");

	ASSERT_EQUALS( true, s1.valid_pos() );
	ASSERT_EQUALS( "Test", s1.readString() );
	ASSERT_EQUALS( false, s1.valid_pos() );

	s1.reset();
	ASSERT( 0 == s1.size() );

	cout << "testStorageReset() PASSED" << endl;
}

void StorageUnitTests::testStorageValidPos()
{
	TraciStorage s1;
	ASSERT_EQUALS( false, s1.valid_pos() );

	TraciStorage s2;
	s2.writeFloat(1.0);
	ASSERT_EQUALS( true, s2.valid_pos() );

	s2.reset();
	ASSERT_EQUALS( false, s2.valid_pos() );

	cout << "testStorageValidPos() PASSED" << endl;
}

void StorageUnitTests::testStorageLoadCharArray()
{
	TraciStorage sHallo;
	sHallo.writeString("Hello world");
	ASSERT_EQUALS( "Hello world", sHallo.readString() );

	TraciStorage newHallo;
	newHallo.writeString("Hello world");
	unsigned char cBuffer[100];

	unsigned int i=0;
	while ( newHallo.valid_pos() && i < sizeof(cBuffer) )
	{
		cBuffer[i] = newHallo.readChar();
		i++;
	}
	TraciStorage s1(cBuffer, i);
	ASSERT_EQUALS( "Hello world", s1.readString() );

	cout << "testStorageLoadCharArray() PASSED" << endl;
}

void StorageUnitTests::testStorageCharToInt()
{
	TraciStorage s;

	ASSERT_EQUALS(0, (int)s.position());
	ASSERT_EQUALS(0, (int)s.size());

	s.writeChar(0);
	ASSERT_EQUALS(0, (int)s.position());
	ASSERT_EQUALS(1, (int)s.size());

	s.writeChar(0);
	ASSERT_EQUALS(0, (int)s.position());
	ASSERT_EQUALS(2, (int)s.size());

	s.writeChar(0);
	ASSERT_EQUALS(0, (int)s.position());
	ASSERT_EQUALS(3, (int)s.size());

	s.writeChar(4);
	ASSERT_EQUALS(0, (int)s.position());
	ASSERT_EQUALS(4, (int)s.size());

	int n = s.readInt();

	ASSERT_EQUALS(4, (int)s.position());
	ASSERT_EQUALS(4, (int)s.size());
	ASSERT_EQUALS(4, n);

	s.reset();

	ASSERT_EQUALS(0, (int)s.position());
	ASSERT_EQUALS(0, (int)s.size());

	s.writeChar(0);
	ASSERT_EQUALS(0, (int)s.position());
	ASSERT_EQUALS(1, (int)s.size());

	s.writeChar(0);
	ASSERT_EQUALS(0, (int)s.position());
	ASSERT_EQUALS(2, (int)s.size());

	s.writeChar(0);
	ASSERT_EQUALS(0, (int)s.position());
	ASSERT_EQUALS(3, (int)s.size());

	s.writeChar(4);
	ASSERT_EQUALS(0, (int)s.position());
	ASSERT_EQUALS(4, (int)s.size());

	n = s.readInt();

	ASSERT_EQUALS(4, (int)s.position());
	ASSERT_EQUALS(4, (int)s.size());
	ASSERT_EQUALS(4, n);

	cout << "testStorageCharToInt() PASSED" << endl;

}

#if 0
void StorageUnitTests::testStorageByteShortInt()
{
	TraciStorage s;
	return;
	try {
//	s.writeByte(-128);
		s.writeByte(127);

//	ASSERT_EQUALS(-128, s.readByte());
		ASSERT_EQUALS(127, s.readByte());

	}
	catch (std::string s)
	{
		std::cerr << s << std::endl;
	}

	/*	int max = 255;
	 int min = 0;

	 byte b;
	 int i;

	 b = (byte) min;
	 i = b;
	 ASSERT_EQUALS(0, i);

	 b = (byte) max;
	 i = b;
	 ASSERT_EQUALS(255, i);
	 */
}
#endif






