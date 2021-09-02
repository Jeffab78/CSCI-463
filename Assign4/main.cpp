/*
Assign 4
Name: Jeff Byerly
Zid : z1860060
Date: 10/22/2020
*/

#include <iostream>
#include <cstdint>
#include <unistd.h>
#include "memory.h"
#include "hex.h"
#include "rv32i.h"

using std::endl;
using std::cerr;


/*
Function: usage()
Use: called to let the user know invalid parameters were entered
Parameters: None
Returns: nothing
*/
static void usage()
{
	cerr << "Usage: rv32i [-m hex-mem-size] infile" << endl;
	cerr<< "	-m specific memory dize (default = 0x10000)" << endl;
	exit(1);
}

/*
function: main()
Use: driver function that can read command line arguments
Parameters: can read in different arguments from the command line
Returns: 0 on success
*/
int main(int argc, char **argv)
{
	uint32_t memory_limit = 0x10000;  //default memory dize =64k
	int opt;

	while ((opt = getopt(argc,argv, "m:")) != -1)
	{
		switch (opt)
		{
			case 'm':
				memory_limit = std::stoul(optarg,nullptr, 16);
				break;
			default: /* '?' */
				usage();
		}
	}

	if (optind >= argc)
		usage();   //missing filename

	memory mem(memory_limit);

	if (!mem.load_file(argv[optind]))
		usage();

	rv32i sim(&mem);
	sim.disasm();
	mem.dump();

	return 0;

}
