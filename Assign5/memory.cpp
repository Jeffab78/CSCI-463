/*
Assignment 5
Name: Jeff Byerly
Zid: z1860060
Date: 11/18/2020
*/

#include "memory.h"
#include "hex.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ios;

/*
Function: memory()
Use: Constructor for memory objects
Parameters: takes a uint32_t that will be the size
Returns: nothing
*/
memory::memory(uint32_t s)
{
	s = (s+15)&0xfffffff0;

	size = s;
	mem = new uint8_t[size];

	for (uint32_t a=0; a<size; ++a)
		mem[a] = 0xa5;
}
/*
Function: ~memory()
Use: Destructor for memory object
Paramaters: none
Returns: nothing
*/
memory::~memory()
{
	delete mem;
}

/*
Function: check_address
Use: checks if the address is in the simulated memory
Parameters: takes in a uint32_t that represents the address the check
Returns: true of false depending on the result
*/
bool memory::check_address(uint32_t i) const
{
	if (!(i<size))
		cout << "WARNING: Address out of range: " << hex0x32(i) << endl;

	return (i<size);
}

/*
Function: get_size()
Use: gets the size of the memory object
Parameters: none
Returns: the size data member
*/
uint32_t memory::get_size() const
{
	return size;
}

/*
Function: get8()
Use: Gets the vlaue of the byte from simulated memory at the given address
Paramters: Takes a uint32_t that represents the current addr
Returns: a uint8_t that is the value of the byte
*/
uint8_t memory::get8(uint32_t addr) const
{
	if (!check_address(addr))
		return 0;

	return mem[addr];
}

/*
Function: get16()
Use: Gets the value of the 2 bytes from simulated memory at given address
Parameters: takes a uint32_t that represents the current address
Returns: a uint16_t that is the value of the 2 bytes
*/
uint16_t memory::get16(uint32_t addr) const
{
	return get8(addr) | get8(addr+1)<<8;
}

/*
Function: get32()
Use: Gets the value of the 4 bytes from simulated memory at the given address
Parameters: takes in a uint32_t that represents the current address
Returns: a uint32_t that is the value of the 4 bytes
*/
uint32_t memory::get32(uint32_t addr) const
{
	return get16(addr) | get16(addr+2)<<16;
}

/*
Function: set8()
Use: Sets the value at the address to the given value
Parameters: takes a uint32_t amd uint8_t that represent the address and value to assign
Returns: nothing
*/
void memory::set8(uint32_t addr, uint8_t val)
{
	if (!check_address(addr))
		return;
	mem[addr] = val;
}

/*
Function: set16()
Use: Sets the value at the address to the given value
Parameters: a uint32_t and uint16_t that represent the address and value to assign it.
Returns: nothing
*/
void memory::set16(uint32_t addr, uint16_t val)
{
	set8(addr, val&0x0ff);
	set8(addr+1, val>>8);
}

/*
Function: set32()
Use: sets the value at the address to the given value
Parameters: 2 uint32_t which represent the address and value to assign it
Returns: nothing
*/
void memory::set32(uint32_t addr, uint32_t val)
{
	set16(addr, val&0x0ffff);
	set16(addr+2, val>>16);
}

/*
Function: dump()
Use: dumps the content of the memory to the output
Parameters: none
Returns: nothing
*/
void memory::dump() const
{
	char ascii[17];
	ascii[16] = 0;

	for (uint32_t i=0; i<size; i++)
	{
		if (i%16 == 0)
		{
			if ( i != 0)
				cout << " *" << ascii <<"*" << endl;
			cout << hex32(i) << ":";
		}

		uint8_t ch = get8(i);
		cout << (i%16==8?"  ":" ") << hex8(ch);
		ascii[i%16] = isprint(ch)?ch:'.';
	}
	cout << " *" << ascii <<"*" << endl;
}

/*
Function: load_file()
Use: opens a file in binary mode and read its content
Parameters: takes in a file name in a string format
Returns: True or false depending on if a file is read successfully
*/
bool memory::load_file(const std::string &fname)
{
	ifstream infile(fname, ios::in|ios::binary);


	if (!infile)
	{
		cerr << "can't open file '" << fname << "' for reading." << endl;
		return false;
	}

	uint8_t i;
	uint32_t addr = 0;

	while (infile.read((char*)&i, 1))
	{
		if (addr>=size)
		{
			cerr << "Program too big." << endl;
			return false;
		}
		set8(addr,i);
		++addr;
	}

	return true;
}



