/*
Assign 5
Name: Jeff Byerly
Zid : z1860060
Date: 11/18/2020
*/

#include <iostream>
#include <cstdint>
#include <unistd.h>
#include <string>
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
	cerr << "Usage: rv32i [-dirz][-m hex-mem-size][-l max number of instrictions] infile" << endl;
	cerr<< "	-m specific memory dize (default = 0x10000)" << endl;
	cerr<< "d = Show a disassembly before program simulation begins" << endl;
	cerr<< "i = Show instruction printing during execution" << endl;
	cerr<< "r = Show a dump of the hart before each instruction" << endl;
	cerr<< "z = Show a dump of hart after the sim has halted" << endl;
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
	bool d = false, i = false, r = false, z = false;
	int limit = 0;

	while ((opt = getopt(argc,argv, "m:l:dirzh?")) != -1)
	{
		switch (opt)
		{
			case 'm':
				memory_limit = std::stoul(optarg,nullptr, 16);
				break;
			case 'l':
				limit = std::stoul(optarg,0,10);
				break;
			case 'd':
				d = true;
				break;
			case 'i':
				i = true;
				break;
			case 'r':
				r = true;
				break;
			case 'z':
				z = true;
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
	sim.set_show_instructions(i);
	sim.set_show_registers(r);
	
	if(d)
	{	
		sim.disasm();
	}
	
	sim.run(limit);
	
	if (z)
	{
		sim.dump();
		mem.dump();
	}
	
	

	return 0;

}
