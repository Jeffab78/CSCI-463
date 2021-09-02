/*
Assignment: 4
Name: Jeff Byerly
Zid: z1860060
Date: 10/22/2020
*/

#include "memory.h"
#include "hex.h"
#include "rv32i.h"

#include <iostream>
#include <iomanip>
#include <sstream>

using std::cout;
using std::ios;
using std::endl;
using std::left;
using std::setw;
using std::setfill;
using std::ostringstream;


/*
Function: rv32i
Use: Saves the argument m in mem variable
Params: takes a pointer
Returns: nothing
*/
rv32i::rv32i(memory *m)
 {
   mem = m;	
 }
 
/*
Function: disasm
Use: This method disassembles the instructions in the simulated memory
Parameters: void
Returns: nothing
*/
void rv32i::disasm(void)
{
	//sets pc to 0 and loop through the size of mem
	for ( pc = 0; pc < mem->get_size();pc+=4 )
		cout << hex32(pc) << ": " <<decode (mem->get32(pc)) << endl;
		
	

	
}

/*
Function: decode
Use: This function returns a string containing the disassembled instruction text
Params: Takes in a uint32_t
Returns: a string
*/
string rv32i::decode(uint32_t insn) const
{
	//varialbes that gets bin codes to check
	uint32_t opcode = get_opcode ( insn );
	uint32_t funct3 = get_funct3 ( insn );
	uint32_t funct7 = get_funct7 ( insn );
	int32_t imm_i = get_imm_i ( insn ) ;
	
	//switch statement that checks for every opcode case
	switch (opcode)
	{
		default: 
			return render_illegal_insn(insn);
		case opcode_lui:
			return render_lui(insn);
		case opcode_auipc:
			return render_auipc(insn);
		case opcode_jal:
			return render_jal(insn);
		case opcode_jalr:
			return render_jalr(insn);
		case opcode_fence:
			return render_fence(insn);
			
		//Checks for r-type opcode
		case opcode_rtype:
			switch (funct3)
			{
				default:
					return render_illegal_insn(insn);
				case funct3_add:
					switch (funct7)
					{
						default: 
							return render_illegal_insn(insn);
						case funct7_add:
							return render_rtype(insn,"add");
						case funct7_sub:
							return render_rtype(insn,"sub");
					}
				case funct3_sll:
					return render_rtype(insn,"sll");
				case funct3_slt:
					return render_rtype(insn,"slt");
				case funct3_sltu:
					return render_rtype(insn,"sltu");
				case funct3_and:
					return render_rtype(insn,"and");
				case funct3_or:
					return render_rtype(insn,"or");
				case funct3_xor:
					return render_rtype(insn,"xor");
				case funct3_srl:
					switch (funct7)
					{
						default:
							return render_illegal_insn(insn);
						case funct7_sra:
							return render_rtype(insn,"sra");
						case funct7_srl:
							return render_rtype(insn,"srl");
					}
			}
		//check for b-type opcode
		case opcode_btype:
			switch (funct3)
			{
				default: 
					return render_illegal_insn(insn);
				case funct3_beq:
					return render_btype(insn,"beq");
				case funct3_bne:
					return render_btype(insn,"bne");
				case funct3_bge:
					return render_btype(insn,"bge");
				case funct3_bgeu:
					return render_btype(insn,"bgeu");
				case funct3_blt:
					return render_btype(insn,"blt");
				case funct3_bltu:
					return render_btype(insn,"bltu");	
			}
			
		//checks for i-type alu opcode
		case opcode_itype_alu:
			switch (funct3)
			{
				default: 
					return render_illegal_insn(insn);
				case funct3_addi:
					return render_itype_alu(insn,"addi",imm_i);
				case funct3_andi:
					return render_itype_alu(insn,"andi",imm_i);
				case funct3_ori:
					return render_itype_alu(insn,"ori",imm_i);
				case funct3_xori:
					return render_itype_alu(insn,"xori",imm_i);
				case funct3_slti:
					return render_itype_alu(insn,"slti",imm_i);
				case funct3_sltiu:
					return render_itype_alu(insn,"sltiu",imm_i);
				case funct3_slli:
					return render_itype_alu(insn,"slli",imm_i&XLEN);
				case funct3_switch:
					switch (funct7)
					{
						default:
							return render_illegal_insn(insn);
						case funct7_srai:
							return render_itype_alu(insn,"srai", imm_i%XLEN);
						case funct7_srli:
							return render_itype_alu(insn,"srli", imm_i%XLEN);
					}
			}
		//checks for i-type load opcode
		case opcode_itype_load:
			switch(funct3)
			{	
				default:
					return render_illegal_insn(insn);
				case funct3_lh:
					return render_itype_load(insn,"lh");
				case funct3_lw:
					return render_itype_load(insn,"lw");
				case funct3_lb:
					return render_itype_load(insn,"lb");	
				case funct3_lhu:
					return render_itype_load(insn,"lhu");
				case funct3_lbu:
					return render_itype_load(insn,"lbu");
			}
		//checks for ebreak opcoke	
		case opcode_ebreak:
			switch(funct7 + get_rs2(insn))
			{
				default: 
					return render_illegal_insn(insn);
					case funct7_ecall:
						return render_ecall(insn);
					case funct7_ebreak:
						return render_ecall(insn);
			}

		case opcode_stype:
			switch (funct3)
			{
				default: 
					return render_illegal_insn(insn);
				case funct3_sh:
					return render_stype(insn,"sh");
				case funct3_sw:
					return render_stype(insn,"sw");
				case funct3_sb:
					return render_stype(insn,"sb");
			}
	}
}
/*
Function: get_opcode
Use: returns the opcode from the passed in argument
Parameters: takes in a uint32_t
Returns: Returns a uint32_t
*/
uint32_t rv32i::get_opcode(uint32_t insn)
{
	return (insn&0x0000007f);
}

/*
function: get_rd
Use: Extracts and returns the rd field
Parameters: takes in a uint32_t
Returns: Returns a uint32_t
*/
uint32_t rv32i:: get_rd(uint32_t insn)
{
	return (insn&0xf80) >> (7-0);
}

/*
Function: get_funct3
Use: Extracts and returns the funct3 field
Params: Takes in a uint32_t
Returns: Returns a uint32_t that is the funct3 field
*/
uint32_t rv32i::get_funct3(uint32_t insn)
{
	return (insn&0x7000) >> 12;
}

/*
function: get_rs1
Use: extracts and returns the rs1 field
Params: Takes in a uint32_t
Returns: Returns a uint32_t that is the rs1 field
*/
uint32_t rv32i::get_rs1(uint32_t insn)
{
	return (insn&0xf8000) >> 15;
}

/*
function: get_rs2
Use: extracts and returns the rs2 field
Params: takes a uint32_t
Returns: returns a uint32_t that is the rs2 field
*/
uint32_t rv32i::get_rs2(uint32_t insn)
{
	return (insn&0x1f00000) >> 20;
}

/*
Function: get_funct7
Use: Extracts and returns the funct7 field
Params: Takes in a uint32_t
Returns: returns a uint32_t that is the funct7 field
*/
uint32_t rv32i::get_funct7(uint32_t insn)
{
	return (insn&0xfe000000) >> 25;
}

/*
Function: get_imm_i
Use: Extracts and returns the imm_i field
Params: takes in a uint32_t
Returns: Returns a int32_t that is the imm_i field
*/
int32_t rv32i::get_imm_i(uint32_t insn)
{
	int32_t temp = (insn&0xfff00000) >> 20;
	 if (insn&0x80000000)
		 temp |= 0xfffff000;
	 
	 return temp;
}

/*
Function: get_imm_u
Use: extracts and returns the imm_u field
Params: takes in a uint32_t
Returns: an int32_t that is the imm_u field
*/
int32_t rv32i::get_imm_u(uint32_t insn)
{
	//converts to signed
	int32_t temp = (insn&0xfffff000);
	return temp;
}

/*
Function: get_imm_b
Use: Extracts and returns the imm_b field
Params: takes in a uint32_t 
Returns: a int32_t that is the imm_b field
*/
int32_t rv32i::get_imm_b(uint32_t insn)
{
	int32_t temp = (insn&0x80) << 4;
	
	temp |= (insn & 0xf00) >> 7;
	temp |= (insn&0x7e000000) >> 20;
	temp |= (insn&0x80000000) >> 19;
	
	if (insn&0x80000000)
		temp |= 0xffffe000;
	
	return temp;
	
}

/*
Function: get_imm_s
Use: extracts and returns the imm_s field
Params: takes in a uint32_t
Returns: returns a int32_t that is the imm_s field
*/
int32_t rv32i::get_imm_s(uint32_t insn)
{
	int32_t temp = (insn&0xfe000000) >> (25-5);
	temp |= (insn&0x00000f80) >> (7-0);

	if (insn&0x80000000)
		temp |= 0xfffff000;
	
	return temp;
}
/*
Function: get_imm_j
Use: Extracts and returns the imm_j field
Params: takes a uint32_t
Returns: returns an int32_t that is the imm_j field
*/
int32_t rv32i::get_imm_j(uint32_t insn)
{
	int32_t temp = (insn&0xff000);  
	
	temp |= (insn&0x80000000) >> (11); 
	temp |= (insn&0x100000) >> (9); 
	temp |= (insn&0x7fe00000) >> (20);  
	
	if (insn&0x80000000)
		temp |= 0xffe00000;
	
	return temp;
}

/*
Function: render_illegal_insn
Use: returns an error message for an unimplemented instruction
Params: a uint32_t
Returns: a string
*/
string rv32i::render_illegal_insn(uint32_t insn) const
{
	ostringstream output;
	
	output << hex32(insn) << " " << std::left << setfill(' ') << setw(mnemonic_width) << "ERROR: UNIMPLEMENTED INSTRUCTION";
	return output.str();
}
/*
function: render_lui
Use: Format for lui
Params: a uint32_t 
Returns: a string
*/
string rv32i::render_lui(uint32_t insn) const
{
	ostringstream output;
	
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);
	
	
	output << hex32(insn) << " " << left << setfill(' ') << setw(mnemonic_width) << "lui" << "x" << std::dec<<rd<< ",0x"<< std::hex << ((imm_u >>12)&0x0fffff);
	
	return output.str();
}
/*
Function: render_auipc
Use: format for auipc
Params: Takes a uint32_t
Returns: a string
*/
string rv32i::render_auipc(uint32_t insn) const
{
	ostringstream output;
	
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);
	
	output << hex32(insn) << " " << left << setfill(' ') << setw(mnemonic_width) << "auipc"<< "x" << std::dec<< rd <<",0x"<< std::hex << ((imm_u >> 12) & 0x0fffff);
	
	return output.str();
}

/*
Function: render_jal
Use: formats for jal
Params: Takes a uint32_t
Returns: a string
*/
string rv32i::render_jal(uint32_t insn) const
{
	ostringstream output;
	
	uint32_t rd = get_rd(insn);
	int32_t imm_j = get_imm_j(insn);
	
	output << hex32(insn) << " " << left << setfill(' ') << setw(mnemonic_width) << "jal"<< "x" << std::dec<< rd << ",0x"<< std::hex << ((imm_j + pc));
	
	return output.str();
}
/*
Function: render_jalr
Use: formats for jalr
Params: takes a uint32_t
Returns: a string
*/
string rv32i::render_jalr(uint32_t insn) const
{
	ostringstream output;
	
	uint32_t rs1 = get_rs1(insn);
	uint32_t rd = get_rd(insn);
	int32_t imm_i = get_imm_i(insn);
	
	output << hex32(insn) << " " << left << setfill(' ') << setw(mnemonic_width) << "jalr"<< "x" << std::dec << rd << "," << imm_i << "(x" << rs1<< ")";
	
	return output.str();
}
/*
Function: render_btype
Use: formats for btype
Params: Takes in a uint32_t and a const char
Returns: a string
*/
string rv32i::render_btype(uint32_t insn, const char *mnemonic) const
{
	ostringstream output;
	
	int32_t imm_b = get_imm_b(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	
	output << hex32(insn) << " " << left << setfill(' ') << setw(mnemonic_width) << mnemonic << "x" << std::dec<<rs1<< ",x" << rs2<< "," << "0x" << std::hex << (imm_b +pc);
	
	return output.str();
	
}
/*
function: render_itype_load
Use: formats output for itype load
Params: Takes in a uint32_t and a const char
Returns: a string
*/
string rv32i::render_itype_load(uint32_t insn, const char *mnemonic) const
{
	ostringstream output;
	
	int32_t imm_i = get_imm_i(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rd = get_rd(insn);
	
	output << hex32(insn) << " " << left << setfill(' ') << setw(mnemonic_width) << mnemonic << "x" << std::dec<< rd << "," << imm_i << "(x" << rs1 << ")";
	
	return output.str();
	
	
}
/*
Function: render_stype
Use: Formats output for stype
Params: Takes in a uint32_t and a const char
Returns: A string
*/
string rv32i::render_stype(uint32_t insn, const char *mnemonic) const
{
	ostringstream output;
	
	int32_t imm_s = get_imm_s(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	
	output << hex32(insn) << " " << left << setfill(' ') << setw(mnemonic_width) << mnemonic << "x" << std::dec << rs2 << "," << imm_s << "(x" <<rs1<< ")";
	
	return output.str();
}

/*
Function: render_itype_alu
Use: formats output for itype alu
Params: Takes in a uint32_t, a const char, and an int32_t
Returns: a string
*/
string rv32i::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i) const
{
	ostringstream output;
	
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t funct3 = get_funct3(insn);
	
	if (funct3 == 0b001 || 0b101)
	{
		output << hex32(insn) << " " << left << setfill(' ') << setw(mnemonic_width) << mnemonic << "x" << std::dec << rd << ",x" << rs1 << "," << ((insn&0x1f00000) >>20);
		
		return output.str();
	}
	else
	{
		output << hex32(insn) << " " << left << setfill(' ') << setw(mnemonic_width) << mnemonic << "x" << std::dec << rd << ",x" << rs1 << "," << imm_i;
		
		return output.str();
	}
}

/*
Function: render_rtype
Use: formats output for rtype
Params:Params: Takes in a uint32_t and a const char
Returns: A string
*/
string rv32i::render_rtype(uint32_t insn, const char *mnemonic) const
{
	ostringstream output;
	
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	uint32_t rd = get_rd(insn);

	output << hex32(insn) << " " << left << setfill(' ') << setw(mnemonic_width) << mnemonic << "x" << std::dec << rd << ",x" << rs1 << ",x" << rs2;
	
	return output.str();
}
/*
Function: render_fence
Use: formats output for fence
Params:Params: Takes in a uint32_t
Returns: A string
*/
string rv32i::render_fence(uint32_t insn) const
{
	ostringstream output;
	
	output <<hex32(insn) << "  " << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "fence" << (insn&0x08000000?"i":"") << 
	(insn&0x04000000?"o":"") << (insn&0x02000000?"r":"") << (insn&0x01000000?"w":"") << "," << (insn&0x00800000?"i":"") << (insn&0x00400000?"o":"") <<
	(insn&0x00200000?"r":"") << (insn&0x00100000?"w":"");
	
	return output.str();
	
	
}	
/*
Function: render_ecall
Use: formats output for ecall
Params:Params: Takes in a uint32_t
Returns: A string
*/
string rv32i::render_ecall(uint32_t insn) const
{
	ostringstream output;
	
	output << hex32(insn) << " " << left << setfill(' ') << setw(mnemonic_width) << "ecall";
	
	return output.str();
}
/*
Function: render_ebreak
Use: formats output for ebreak
Params:Params: Takes in a uint32_t
Returns: A string
*/
string rv32i::render_ebreak(uint32_t insn) const
{
	ostringstream output;
	
	output << hex32(insn) << " " << left << setfill(' ') << setw(mnemonic_width) << "ebreak";
	
	return output.str();
}

	
	
	
			
