/*
Assignment: 5
Name: Jeff Byerly
Zid: z1860060
Date: 11/18/2020
*/

#ifndef rv32i_H
#define rv32i_H

#include <string>
#include <iostream>
#include "registerfile.h"

using std::string;
using std::ostream;

class rv32i
{
	//member variables
	public: 
	memory *mem;
	uint32_t pc = 0;
	registerfile regs;
	bool halt;
	bool show_instructions;
	bool show_registers;
	uint64_t insn_counter;
	
	

	//Consts used for opcodes
	static constexpr uint32_t XLEN = 32;
	static constexpr int mnemonic_width = 8;
	static constexpr int instruction_width = 35;
	static constexpr uint32_t opcode_lui = 0b0110111;
	static constexpr uint32_t opcode_auipc = 0b0010111;
	static constexpr uint32_t opcode_jal = 0b1101111;
	static constexpr uint32_t opcode_jalr = 0b1100111;
	static constexpr uint32_t opcode_fence = 0b0001111;
	static constexpr uint32_t opcode_btype = 0b1100011;
	static constexpr uint32_t opcode_rtype = 0b0110011;
	static constexpr uint32_t opcode_stype = 0b0100011;
	static constexpr uint32_t opcode_itype_alu =0b0010011;
	static constexpr uint32_t opcode_itype_load =0b0000011;
	static constexpr uint32_t opcode_load_imm = 0b0000011;
	static constexpr uint32_t opcode_ebreak = 0b1110011;
	static constexpr uint32_t funct3_sll = 0b001;
	static constexpr uint32_t funct3_slt = 0b010;
	static constexpr uint32_t funct3_sltu = 0b011;
	static constexpr uint32_t funct3_and = 0b111;
	static constexpr uint32_t funct3_or = 0b110;
	static constexpr uint32_t funct3_xor = 0b100;
	static constexpr uint32_t funct3_srl = 0b101;
	static constexpr uint32_t funct3_addi = 0b000;
	static constexpr uint32_t funct3_add = 0b000;
	static constexpr uint32_t funct3_andi = 0b111;
	static constexpr uint32_t funct3_ori = 0b110;
	static constexpr uint32_t funct3_xori = 0b100;
	static constexpr uint32_t funct3_slti = 0b010;
	static constexpr uint32_t funct3_sltiu = 0b011;
	static constexpr uint32_t funct3_slli = 0b001;
	static constexpr uint32_t funct3_switch = 0b101;
	static constexpr uint32_t funct3_lh = 0b001;
	static constexpr uint32_t funct3_lw = 0b010;
	static constexpr uint32_t funct3_lb = 0b000;
	static constexpr uint32_t funct3_lhu = 0b101;
	static constexpr uint32_t funct3_lbu = 0b100;
	static constexpr uint32_t funct3_sh = 0b001;
	static constexpr uint32_t funct3_sw = 0b010;
	static constexpr uint32_t funct3_sb = 0b000;
	static constexpr uint32_t funct3_beq = 0b000;
	static constexpr uint32_t funct3_bne = 0b001;
	static constexpr uint32_t funct3_bge = 0b101;
	static constexpr uint32_t funct3_bgeu = 0b111;
	static constexpr uint32_t funct3_blt = 0b100;
	static constexpr uint32_t funct3_bltu = 0b110;
	static constexpr uint32_t funct7_srai = 0b0100000;
	static constexpr uint32_t funct7_srli = 0b0000000;
	static constexpr uint32_t funct7_sra = 0b0100000;
	static constexpr uint32_t funct7_srl = 0b0000000;
	static constexpr uint32_t funct7_add = 0b0000000;
	static constexpr uint32_t funct7_sub = 0b0100000;
	static constexpr uint32_t funct7_ecall = 0b000000000000;
	static constexpr uint32_t funct7_ebreak = 0b000000000001;
		


	rv32i(memory *m);
	void disasm(void);
	void dcex(uint32_t insn, ostream *out);
	void ticcd ..k();
	void run(uint64_t limit);
	void reset();
	void dump() const;
	bool is_halted() const;
	void set_show_instructions(bool b);
	void set_show_registers(bool b);
	
	
	//instruction formatting for printing
	string decode(uint32_t insn) const;
	string render_illegal_insn(uint32_t insn) const;
	string render_lui(uint32_t insn) const;
	string render_auipc(uint32_t insn) const;
	string render_jal(uint32_t insn) const;
	string render_jalr(uint32_t insn) const;
	string render_btype(uint32_t insn, const char *mnemonic) const;
	string render_itype_load(uint32_t insn, const char *mnemonic) const;
	string render_stype(uint32_t insn, const char *mnemonic) const;
	string render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i) const;
	string render_rtype(uint32_t insn, const char *mnemonic) const;
	string render_fence(uint32_t insn) const;
	string render_ecall(uint32_t insn) const;
	string render_ebreak(uint32_t insn) const;
	
	//exec functions
	void exec_illegal_insn(uint32_t insn, ostream * pos);
	void exec_lui(uint32_t insn, ostream * pos);
	void exec_auipc(uint32_t insn, ostream * pos);
	void exec_jal(uint32_t insn, ostream * pos);
	void exec_jalr(uint32_t insn, ostream * pos);
	void exec_btype(uint32_t insn, const char *mnemonic, ostream * pos, uint32_t funct3);
	void exec_rtype(uint32_t insn, const char *mnemonic, ostream* pos, uint32_t funct3);
	void exec_itype_load(uint32_t insn, const char *mnemonic, ostream* pos, uint32_t funct3);
	void exec_stype(uint32_t insn, const char *mnemonic, ostream* pos, uint32_t funct3);
	void exec_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i, ostream* pos, uint32_t funct3);
	void exec_ebreak(uint32_t insn, ostream* pos);
	void exec_ecall(uint32_t insn, ostream* pos);
	void exec_fence(uint32_t insn, ostream* pos);
	
	
	
	
	//static member functions
	static uint32_t get_opcode(uint32_t insn);
	static uint32_t get_rd(uint32_t insn);
	static uint32_t get_funct3(uint32_t insn);
	static uint32_t get_rs1(uint32_t insn);
	static uint32_t get_rs2(uint32_t insn);
	static uint32_t get_funct7(uint32_t insn);
	static int32_t get_imm_i(uint32_t insn);
	static int32_t get_imm_u(uint32_t insn);
	static int32_t get_imm_b(uint32_t insn);
	static int32_t get_imm_s(uint32_t insn);
	static int32_t get_imm_j(uint32_t insn);
};
#endif
