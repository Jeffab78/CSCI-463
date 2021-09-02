/*
Assignment: 5
Name: Jeff Byerly
Zid: z1860060
Date: 11/18/2020
*/

#include "memory.h"
#include "hex.h"
#include "rv32i.h"
#include "registerfile.h"

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

/*
Function: exec_ebreakU
Use: executes ebreak function
Params: uint32_t and an ostream *
Returns: nothing
*/
void rv32i::exec_ebreak(uint32_t insn, ostream* pos)
{
	if (pos)
	{
		string oput = render_ebreak(insn);
		oput.resize(instruction_width,' ');
		*pos << oput << "// HALT" << endl;
	}
	halt = true;
	cout << "Execution terminated by EBREAK instruction";
	
	pc -=4;
}

/*
function: exec_ecall
Use: executes ecall funct
Params: a uint32_t and ostream *
Returns: nothing
*/
void rv32i::exec_ecall(uint32_t insn, ostream * pos)
{
	if (pos)
	{
		string oput = render_ecall(insn);
		oput.resize(instruction_width,' ');
		*pos << oput << "// HALT" << endl;
	}
}

/*
function: exec_fence
Use: executes fence
Params: a uint32_t and ostream *
Returns: nothing
*/
void rv32i::exec_fence(uint32_t insn, ostream * pos)
{
	if (pos)
	{
		string oput = render_fence(insn);
		oput.resize(instruction_width,' ');
		*pos << oput << "// fence" << endl;
	}
}
/*
function: exec_btype
Use: simulates execution of btype instructions
Params: a uint32_t, a const char, an ostream *, and a uint32_t
Returns: nothing
*/
void rv32i::exec_btype(uint32_t insn, const char *mnemonic, std::ostream* pos, uint32_t funct3)
{
	int32_t imm_b = get_imm_b(insn) - 4;
	int32_t tempPc = pc;
	
	uint32_t rs1 = regs.get(get_rs1(insn));
	uint32_t rs2 = regs.get(get_rs2(insn));
	
	switch (funct3)
	{
		case funct3_blt:
			if ((int32_t)rs1 < (int32_t)rs2) 
				tempPc += imm_b;
			break;
		case funct3_bge:
			if ((int32_t)rs1 >= (int32_t)rs2) 
				tempPc += imm_b;
			break;
		case funct3_bltu:
			if (rs1 < rs2)
				tempPc += imm_b;
			break;
		case funct3_bgeu:
			if (rs1 >= rs2)
				tempPc += imm_b;
			break;
		case funct3_beq:
			if (rs1 == rs2)
				tempPc += imm_b;
			break;
		case funct3_bne:
			if (rs1 != rs2)
				tempPc += imm_b;
			break;
	}
	
	if (pos)
	{
		static const char* n[] = { " == ", " != ", "", "", " < ", " >=", " <U ", " >=U " };
		pc -=4;
		string btypeOut = render_btype(insn, mnemonic);
		
		pc +=4;
		btypeOut.resize(instruction_width, ' ');
		*pos << btypeOut << "// pc += (" <<hex0x32(rs1) << n[funct3] << hex0x32(rs2) << " ? " << hex0x32(imm_b +4);
		*pos << " : 4) = " <<hex0x32(tempPc);
	}
	pc = tempPc;
}

/*
function: exec_stype
Use: executes Stype functions
Params: a uint32_t, a const char *, an ostream *, and a uint32_t
Returns: nothing
*/
void rv32i::exec_stype(uint32_t insn, const char *mnemonic, std::ostream* pos, uint32_t funct3)
{
	int32_t imm_s = get_imm_s(insn);
	uint32_t rs1 = regs.get(get_rs1(insn));
	uint32_t rs2 = regs.get(get_rs2(insn));
	uint32_t add = rs1+imm_s;
	
	const char *pre;
	
	switch (funct3)
	{
		case funct3_sb:
			mem->set8(add,rs2);
			rs2=(uint8_t)rs2;
			pre="m8(";
			break;
		case funct3_sh:
			mem->set16(add,rs2);
			rs2=(uint16_t)rs2;
			pre="m16(";
			break;
		case funct3_sw:
			mem->set32(add,rs2);
			pre="m32(";
			break;
	}
	
	if (pos)
	{
		string oput = render_stype(insn,mnemonic);
		oput.resize(instruction_width,' ');
		*pos << oput << "// " << pre << hex0x32(rs1) << " + " <<hex0x32(imm_s);
		*pos << ") = " << hex0x32(rs2);
	}
}
/*
function:exec_itype_alu
Use: executes the alu itype functions
Params:a uint32_t, a const char *, an ostream *, and a uint32_t
Returns: nothing
*/
void rv32i::exec_itype_alu(uint32_t insn,const char* mnemonic, int32_t imm_i,ostream * pos,uint32_t funct3)
{
	imm_i = get_imm_i(insn);
	uint32_t rs1=regs.get(get_rs1(insn));
	uint32_t rd = get_rd(insn);
	uint32_t value;
	const char *pre = "", *suf ="";
	
	bool hex = true, shift = false;
	
	switch (funct3)
	{
		case funct3_addi:
			value = imm_i + rs1;
			break;
		case funct3_slti:
			pre = "(";
			suf = ") ? 1 : 0";
			value = (int32_t)rs1 < imm_i;
			hex =false;
			break;
		case funct3_sltiu:
			pre = "(";
			suf = ") ? 1 : 0";
			value = rs1 < (uint32_t)imm_i;
			hex =false;
			break;
		case funct3_slli:
			shift = true;
			value = rs1 << imm_i;
			break;
		case funct3_andi:
			value = rs1 & imm_i;
			break;
		case funct3_ori:
			value = rs1 | imm_i;
			break;
		case funct3_xori:
			value = rs1 ^ imm_i;
			break;
		case 8:
			shift = true;
			value = rs1>>imm_i;
			break;
		case 9:
			shift = true;
			value = (int32_t)rs1 >> imm_i;
			break;
			
	}
	
	regs.set(rd,value);
	
	if (pos)
	{
		static const char* opr[] = {" + ", " << "," < "," <U "," ^ ",""," | "," & "," >> "," >> "};
		string oput = render_itype_alu(insn,mnemonic, imm_i);
		oput.resize(instruction_width, ' ');
		
		*pos << oput << "// x" << rd <<" = "<< pre <<hex0x32(rs1) <<opr[funct3];
		
		if(shift)
			*pos << (imm_i &31);
		else
		{
			if (hex)
				*pos << hex0x32(imm_i);
			else
				*pos << imm_i;
		}
		*pos << suf << " = " << hex0x32(value);
	}
		
	
}
/*
function:exec_itype_load
Use: executed itype load function
Params: uint32_t, a const char *, an ostream *, and a uint32_t
Returns: nothing
*/
void rv32i::exec_itype_load(uint32_t insn, const char * mnemonic, ostream *pos, uint32_t funct3)
{
	int32_t imm_i = get_imm_i(insn);
	uint32_t rs1=regs.get(get_rs1(insn));
	uint32_t rd = get_rd(insn);
	uint32_t value;
	uint32_t addr = imm_i+rs1;
	
	switch ( funct3)
	{
		case funct3_lw:
			value = mem->get32(addr);
			break;
		case funct3_lh:
			value = (int16_t)mem->get16(addr);
			break;
		case funct3_lb:
			value = (int8_t)mem->get8(addr);
			break;
		case funct3_lhu:
			value = mem->get16(addr);
			break;
		case funct3_lbu:
			value = mem->get8(addr);
			break;
	}
	
	regs.set(rd,value);
	
	if (pos)
	{
		static const char* t[] = { "sx(m8(","sx(m16(","sx(m32(","","zx(m8(","zx(m16(" };
		string oput = render_itype_load(insn,mnemonic);
		oput.resize(instruction_width,' ');
		
		*pos << oput << "// x" << rd<< " = " <<t[funct3]<< hex0x32(rs1) << " + " <<hex0x32(imm_i) << ")) = " <<hex0x32(value);
	}
}
/*
function: exec_rtype
Use: executes rtype functions
Params: uint32_t, a const char *, an ostream *, and a uint32_t
returns: nothing
*/
void rv32i::exec_rtype(uint32_t insn,const char* mnemonic,ostream* pos, uint32_t funct3)
{
	int32_t value;
	uint32_t rs1=regs.get(get_rs1(insn));
	uint32_t rs2=regs.get(get_rs2(insn));
	uint32_t rd = get_rd(insn);	
	const char *pre = "", *suf = "";
	bool shift = false;
	
	switch (funct3)
	{
		case funct3_andi:
			value = rs1 & rs2;
			break;
		case funct3_addi:
			value = rs1+rs2;
			break;
		case funct3_slti:
			pre="(";
			suf=") ? 1 :0";
			value = (int32_t)rs1 < (int32_t)rs2;
			break;
		case funct3_sltiu:
			pre="(";
			suf=") ? 1 :0";
			value = (uint32_t)rs1 < (uint32_t)rs2;
			break;
		case funct3_slli:
			shift = true;
			value = rs1 << rs2;
			break;
		case funct3_ori:
			value = rs1 | rs2;
			break;
		case funct3_xori:
			value = rs1 ^ rs2;
			break;
		case 8:
			value = rs1 - rs2;
			break;
		case 9:
			shift = true;
			value = rs1 >> rs2;
			break;
		case 10:
			shift = true;
			value = (int32_t)rs1 >> rs2;
			break;
	}
	
	if (pos)
	{
		static const char* opr[] = {" + ", " << "," < "," <U "," ^ ",""," | "," & "," - "," >> "," >> "};
		
		string oput = render_rtype(insn,mnemonic);
		oput.resize(instruction_width,' ');
		
		*pos << oput << "// x" << rd <<" = "<< pre << hex0x32(rs1) << opr[funct3];
		
		if (shift)
			*pos << (rs2 &31);
		else
			*pos <<hex0x32(rs2);
		
		*pos << suf << " = " <<hex0x32(value);
		
	}
		
	regs.set(rd,value);
	
	
}
/*
Function: exec_jal()
Use: executes jal functions
Params: a uint_32t and ostream
Returns: nothing
*/
void rv32i::exec_jal(uint32_t insn, ostream *pos)
{
	int32_t imm_j = get_imm_j(insn);
	uint32_t rd = get_rd(insn);
	
	if (pos)
	{
		pc -=4;
		string oput = render_jal(insn);
		pc +=4;
		oput.resize(instruction_width, ' ');
		*pos << oput << "// x" << rd << " = " << hex0x32(pc) << ", pc = " << hex0x32(pc-4) << " + " << hex0x32(imm_j) << " = " << hex0x32(pc - 4 + imm_j);
	}
	
	regs.set(rd,pc);
	pc +=imm_j - 4;
	
}

/*
Function: exec_jalr()
Use: executes jalr functions
Params: a uint_32t and ostream
Returns: nothing
*/
void rv32i::exec_jalr(uint32_t insn, ostream *pos)
{
	uint32_t rs1 = regs.get(get_rs1(insn));
	uint32_t rd = get_rd(insn);
	int32_t imm_i = get_imm_i(insn);
	
	if (pos)
	{
		string oput = render_jalr(insn);
		oput.resize(instruction_width, ' ');
		
		*pos << oput<<"// x" << rd <<" = "<<hex0x32(pc)<<"c pc = (" << hex0x32(imm_i) << " + " <<hex0x32(rs1) << ") & 0xfffffffe = " <<hex0x32((rs1+imm_i) & 0xfffffffe);
		
	}
	
	regs.set(rd,pc);
	pc = (rs1+imm_i) & 0xfffffffe;
}

/*
Function: exec_lui
Use: executes lui functions
Params: a uint32_t and ostream
Returns: nothing
*/
void rv32i::exec_lui(uint32_t insn, ostream * pos)
{
	int32_t imm_u = get_imm_u(insn);
	uint32_t rd = get_rd(insn);
	
	if (pos)
	{
		string oput = render_lui(insn);
		oput.resize(instruction_width,' ');
		*pos << oput<< "// x" << rd << " = " <<hex0x32(imm_u);
		
	}
	
	regs.set(rd,imm_u);
}

/*
Function: exec_auipc
Use: executes auipc functions
Params: a uint32_t and ostream
Returns: nothing
*/
void rv32i::exec_auipc(uint32_t insn, ostream * pos)
{
	int32_t imm_u = get_imm_u(insn);
	uint32_t rd = get_rd(insn);
	
	if (pos)
	{
		string oput = render_auipc(insn);
		oput.resize(instruction_width,' ');
		
		*pos <<oput << "// x"<< rd << " = " << hex0x32(pc - 4) <<" + " <<hex0x32(imm_u) << " = " <<hex0x32(pc-4 + imm_u);
	}
	
	regs.set(rd,pc - 4 + imm_u);
}

/*
Function: set_show_instructions()
Use: sets flag to show instructions as program executes
Params: a bool
Returns: nothing
*/
void rv32i::set_show_instructions(bool b)
{
	show_instructions = b;
}

/*
Function: set_show_registers()
Use: sets flag to show registers
Params: a bool
Returns: nothing
*/
void rv32i::set_show_registers(bool b)
{
	show_registers = b;
}

/*
Function: is_halted()
Use: accessor for halt member
Params: none
Returns: a bool
*/
bool rv32i::is_halted() const
{
	return halt;
}

/*
Function: reset()
Use: resets the rv32i object and registerfile
Params: none
Returns: nothing
*/
void rv32i::reset()
{
	pc = 0;
	halt = 0;
	insn_counter = 0;
	regs.reset();
}

/*
Function: dump()
Use: dumps the state of the hart
Params: none
Returns: nothing
*/
void rv32i::dump() const
{
	regs.dump();
	cout << " pc " << hex32(pc) << endl;
}

void rv32i::dcex(uint32_t insn, std::ostream* out)
{
//varialbes that gets bin codes to check
	uint32_t opcode = get_opcode ( insn );
	uint32_t funct3 = get_funct3 ( insn );
	uint32_t funct7 = get_funct7 ( insn );
	int32_t imm_i = get_imm_i ( insn ) ;
	
	
	if (show_registers)
		dump();
	
	if (out)
	{
		*out << hex32(pc) << ": ";
	}
	
	pc +=4;
	
	//switch statement that checks for every opcode case
	switch (opcode)
	{
		default: 
			exec_illegal_insn(insn,out);
			return;
		case opcode_lui:
			exec_lui(insn,out);
			return;
		case opcode_auipc:
			exec_auipc(insn,out);
			return;
		case opcode_jal:
			exec_jal(insn,out);
			return;
		case opcode_jalr:
			exec_jalr(insn,out);
			return;
		case opcode_fence:
			exec_fence(insn,out);
			return;
			
		//Checks for r-type opcode
		case opcode_rtype:
			switch (funct3)
			{
				default:
					exec_illegal_insn(insn,out);
					return;
				case funct3_add:
					switch (funct7)
					{
						default: 
							exec_illegal_insn(insn,out);
							return;
						case funct7_add:
							exec_rtype(insn,"add",out,funct3);
							return;
						case funct7_sub:
							exec_rtype(insn,"sub",out,8);
							return;
					}
				case funct3_sll:
					exec_rtype(insn,"sll",out,funct3);
					return;
				case funct3_slt:
					exec_rtype(insn,"slt",out,funct3);
					return;
				case funct3_sltu:
					exec_rtype(insn,"sltu",out,funct3);
					return;
				case funct3_and:
					exec_rtype(insn,"and",out,funct3);
					return;
				case funct3_or:
					exec_rtype(insn,"or",out,funct3);
					return;
				case funct3_xor:
					exec_rtype(insn,"xor",out,funct3);
					return;
				case funct3_srl:
					switch (funct7)
					{
						default:
							exec_illegal_insn(insn,out);
							return;
						case funct7_sra:
							exec_rtype(insn,"sra",out,10);
							return;
						case funct7_srl:
							exec_rtype(insn,"srl",out,9);
							return;
					}
			}
		//check for b-type opcode
		case opcode_btype:
			switch (funct3)
			{
				default: 
					exec_illegal_insn(insn,out);
					return;
				case funct3_beq:
					exec_btype(insn,"beq",out,funct3);
					return;
				case funct3_bne:
					exec_btype(insn,"bne",out,funct3);
					return;
				case funct3_bge:
					exec_btype(insn,"bge",out,funct3);
					return;
				case funct3_bgeu:
					exec_btype(insn,"bgeu",out,funct3);
					return;
				case funct3_blt:
					exec_btype(insn,"blt",out,funct3);
					return;
				case funct3_bltu:
					exec_btype(insn,"bltu",out,funct3);
					return;	
			}
			
		//checks for i-type alu opcode
		case opcode_itype_alu:
			switch (funct3)
			{
				default: 
					exec_illegal_insn(insn,out);
					return;
				case funct3_addi:
					exec_itype_alu(insn,"addi",imm_i,out,funct3);
					return;
				case funct3_andi:
					exec_itype_alu(insn,"andi",imm_i,out,funct3);
					return;
				case funct3_ori:
					exec_itype_alu(insn,"ori",imm_i,out,funct3);
					return;
				case funct3_xori:
					exec_itype_alu(insn,"xori",imm_i,out,funct3);
					return;
				case funct3_slti:
					exec_itype_alu(insn,"slti",imm_i,out,funct3);
					return;
				case funct3_sltiu:
					exec_itype_alu(insn,"sltiu",imm_i,out,funct3);
					return;
				case funct3_slli:
					exec_itype_alu(insn,"slli",imm_i%XLEN,out,funct3);
					return;
				case funct3_switch:
					switch (funct7)
					{
						default:
							exec_illegal_insn(insn,out);
							return;
						case funct7_srai:
							exec_itype_alu(insn,"srai", imm_i%XLEN,out,9);
							return;
						case funct7_srli:
							exec_itype_alu(insn,"srli", imm_i%XLEN,out,8);
							return;
					}
			}
		//checks for i-type load opcode
		case opcode_itype_load:
			switch(funct3)
			{	
				default:
					exec_illegal_insn(insn,out);
					return;
				case funct3_lh:
					exec_itype_load(insn,"lh",out,funct3);
					return;
				case funct3_lw:
					exec_itype_load(insn,"lw",out,funct3);
					return;
				case funct3_lb:
					exec_itype_load(insn,"lb",out,funct3);
					return;	
				case funct3_lhu:
					exec_itype_load(insn,"lhu",out,funct3);
					return;
				case funct3_lbu:
					exec_itype_load(insn,"lbu",out,funct3);
					return;
			}
		//checks for ebreak opcode	
		case opcode_ebreak:
			switch(funct7 + get_rs2(insn))
			{
				default: 
					exec_illegal_insn(insn,out);
					return;
				case funct7_ecall:
					exec_ecall(insn,out);
					return;
				case funct7_ebreak:
					exec_ebreak(insn,out);
					return;
			}
		//checks for stype
		case opcode_stype:
			switch (funct3)
			{
				default: 
					exec_illegal_insn(insn,out);
					return;
				case funct3_sh:
					exec_stype(insn,"sh",out,funct3);
					return;
				case funct3_sw:
					exec_stype(insn,"sw",out,funct3);
					return;
				case funct3_sb:
					exec_stype(insn,"sb",out,funct3);
					return;
			}
	}	
}
/*
Function: exec_illegal_insn
Use: called when an illegal insn is given
Params: a uint32_t and ostream
Returns: nothing
*/
void rv32i::exec_illegal_insn(uint32_t insn, std::ostream* pos)
{
	halt = true;
	
	if (pos)
	{
		string oput = render_illegal_insn(insn);
		oput.resize(instruction_width, ' ');
		*pos << oput << " /// ILLEGAL INSTRUCTION" << endl;
	}
	cout << "Execution Terminated";
	
}

/*
Function: tick()
Use: simulated the exection of a sinlge instruction
Params: none
Returns: nothing
*/
void rv32i::tick()
{
	if (halt)
		return;
	insn_counter++;
	
	if (show_registers)
		dump();
	
	if (!mem->check_address(pc))
	{
		halt = true; 
		return;
	}
	
	
	
	uint32_t insn = mem->get32(pc);
	
	if (show_instructions)
	{
		dcex(insn, &cout);
		cout << endl;
	}
	else
		dcex(insn,nullptr);
	
}
/*
Function: run
Use: runs the simlation with specified mem limit
Params: a uint64_t limit
Returns: nothing
*/
void rv32i::run(uint64_t limit)
{
	//resets every run
	reset();
	
	regs.set(2,mem->get_size());
	
	while (!halt)
	{
		tick();
		if (++insn_counter == limit)
			break;
	}
	
	cout << insn_counter << " instructions executed" << endl;
	
}
	
	
	
			
