/*
Assign 3
Name: Jeff Byerly
Zid : z1860060
Date: 10/2/2020
*/

#include <iostream>
#include <cstdint>
#include "memory.h"
#include "hex.h"

/*
Function: usage()
Use: called to let the user know invalid parameters were entered
Parameters: None
Returns: nothing
*/
void usage()
{
	std::cout << "Please enter the correct parameters" << std::endl;
	exit(-1);
}

/*
function: main()
Use: driver function that can read command line arguments
Parameters: can read in different arguments from the command line
Returns: 0 on success
*/
int main(int argc, char **argv)
{
	if (argc != 3)
		usage();

	memory mem(std::stoul (argv[1],0,16));
	mem.dump();

	if (!mem.load_file(argv[2]))
		usage();
	mem.dump();

	std::cout << mem.get_size() << std::endl;
	std::cout << hex32(mem.get8(0)) << std::endl;
	std::cout << hex32(mem.get16(0)) << std::endl;
	std::cout << hex32(mem.get32(0)) << std::endl;
	std::cout << hex0x32(mem.get8(0)) <<  std::endl;
	std::cout << hex0x32(mem.get16(0)) << std::endl;
	std::cout << hex0x32(mem.get32(0)) << std::endl;
	std::cout << hex8(mem.get8(0)) << std::endl;
	std::cout << hex8(mem.get16(0)) << std::endl;
	std::cout << hex8(mem.get32(0)) << std::endl;

	std::cout << hex0x32(mem.get32(0x1000)) << std::endl;

	mem.set8(0x10, 0x12);
	mem.set16(0x14, 0x1234);
	mem.set32(0x18, 0x87654321);
	mem.dump();

	return 0;

}
