/*
Assign 5
Name: Jeff Byerly
Zid: z1860060
Date: 11/18/20
*/

#include "memory.h"
#include "hex.h"
#include "registerfile.h"

#include <cstring>
#include <iostream>

using std::cout;
using std::endl;

/*
Function: registerfile()
Use: constructor
Params: none
Returns: nothing
*/
registerfile::registerfile()
{
	reset();
}

/*
function: reset()
Use: sets all registers to 0
Params: none
Returns: nothing
*/
void registerfile::reset()
{
	memset(&rfArray, 0xf0, 32*sizeof(int32_t));
	rfArray[0] = 0;
}	

/*
Function: set()
Use: sets the register to val
Params: a register index and val to be inserted
Returns: nothing
*/
void registerfile::set(uint32_t r, int32_t val)
{
	if (r)
		rfArray[r]=val;
}

/*
Function: get()
Use: gets the value at indexed reg
Params: an index to get a val from
Returns: the value at specifcied index
*/
int32_t registerfile::get(uint32_t r) const
{
	if (r)
		return rfArray[r];
	else
		return 0;
}

/*
Function: dump()
Use: dumps the registers to std output_iterator
Params: none
Returns: nothing
*/
void registerfile::dump() const
{
	for (int reg= 0; reg <32; reg +=8)
	{
		cout << (reg < 10 ? " x" : "x") << reg;
		
		for ( int i = reg; i < reg +8; i++)
		{
			cout << ' ' << hex32(rfArray[i]);
		}
		cout << endl;
	}
}
	



	
