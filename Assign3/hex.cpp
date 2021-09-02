/*
Assignment 3
Name: Jeff Byerly
Zid: z1860060
Date: 10/2/2020
*/

#include "hex.h"

#include <iomanip>
#include <sstream>
#include <string>
#include <stdint.h>

/*
Function: hex8()
Use: Turns the input param in 2 hex digits representing the least significant 8 bits
Params: Takes in an int
Returns: a 2 hex digit string
*/
std:: string hex8(uint8_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
	return os.str();
}

/*
Function: hex0x32()
Use: Turns an int into an 8 hex representation
Params: takes in an int
Returns: a string version of the number
*/
std::string hex0x32(uint32_t i)
{
	return std::string("0x")+hex32(i);
}

/*
Function: hex32()
Use: Turns the int into a 8 hex digit representation without the leading 0x
Params: takes in an int
Returns: a string with 8 hex digits
*/
std::string hex32(uint32_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(8) << i;
	return os.str();
}

