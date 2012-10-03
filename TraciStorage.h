/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>                  **
 ** \author Bjoern Hendriks <hendriks@ibr.cs.tu-bs.de>                 **
 **                                                                    **
 ************************************************************************/

/************************************************************************
 * Modification history :
 * 10/03/2012 - Rudhir
 ************************************************************************/

#ifndef __TRACI_CLIENT_STORAGE_H
#define __TRACI_CLIENT_STORAGE_H

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

class TraciStorage {

public:
	typedef std::vector<unsigned char> StorageType;

private:
	StorageType store;
	StorageType::const_iterator iter_;

	bool bigEndian_;

	/// Used in constructors to initialize local variables
	void init();

	/// Check if the next num bytes can be read safely
	void checkReadSafe(unsigned int num) const throw (std::invalid_argument);
	/// Read a byte without validity check
	unsigned char readCharUnsafe();
	/// Write size elements of array begin according to endianness
	void writeByEndianess(const unsigned char * begin, unsigned int size);
	/// Read size elements into array according to endianness
	void readByEndianess(unsigned char * array, int size);

public:

	/// Standard Constructor
	TraciStorage();
	/// Constructor, that fills the storage with an char array. If length is
	// -1, the whole array is handed over
	TraciStorage(const unsigned char[], int length = -1);
	virtual ~TraciStorage();

	virtual bool valid_pos();
	virtual unsigned int position() const;

	// Reads
	virtual unsigned char readChar() throw (std::invalid_argument);
	virtual int readByte() throw (std::invalid_argument);
	virtual int readUnsignedByte() throw (std::invalid_argument);
	virtual int readShort() throw (std::invalid_argument);
	virtual std::string readString() throw (std::invalid_argument);
	virtual std::vector<std::string> readStringList()
			throw (std::invalid_argument);
	virtual int readInt() throw (std::invalid_argument);
	virtual float readFloat() throw (std::invalid_argument);
	virtual double readDouble() throw (std::invalid_argument);

	// Writes
	virtual void writeChar(unsigned char) throw ();
	virtual void writeByte(int) throw (std::invalid_argument);
    // virtual void writeByte(unsigned char) throw();
	virtual void writeUnsignedByte(int) throw (std::invalid_argument);
	virtual void writeShort(int) throw (std::invalid_argument);
	virtual void writeString(const std::string& s) throw ();
	virtual void writeStringList(const std::vector<std::string> &s) throw ();
	virtual void writeInt(int) throw ();
	virtual void writeFloat(float) throw ();
	virtual void writeDouble(double) throw ();
	virtual void writePacket(unsigned char* packet, int length);
	virtual void writePacket(const std::vector<unsigned char> &packet);
	virtual void writeStorage(TraciStorage& store);

	void reset();
	std::string hexDump() const;

	StorageType::size_type size() const {
		return store.size();
	}
	StorageType::const_iterator begin() const {
		return store.begin();
	}
	StorageType::const_iterator end() const {
		return store.end();
	}

};

#endif //__TRACL_CLIENT_STORAGE_H
