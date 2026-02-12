#pragma once

#include <cstdint>
#include <tuple>
#include <string>

#include "instrtype.h"


enum r_types : uint8_t
{
	SYSCALL,
	ADD,
	AND,
	JR,
	NOR,
	OR,
	SLT,
	SLL,
	SRL,
	SUB,
	MFHI,
	MFLO,
	MULT,
	SRA,
	ADDU,
	SLTU,
	SUBU,
	MULTU
};

static r_types get_r_type(const std::string& text)
{
	if (text == "add")
		return ADD;
	if (text == "addu")
		return ADDU;
	if (text == "and")
		return AND;
	if (text == "jr")
		return JR;
	if (text == "nor")
		return NOR;
	if (text == "or")
		return OR;
	if (text == "slt")
		return SLT;
	if (text == "sltu")
		return SLTU;
	if (text == "sll")
		return SLL;
	if (text == "srl")
		return SRL;
	if (text == "sub")
		return SUB;
	if (text == "subu")
		return SUBU;
	if (text == "mfhi")
		return MFHI;
	if (text == "mflo")
		return MFLO;
	if (text == "mult")
		return MULT;
	if (text == "multu")
		return MULTU;
	if (text == "sra")
		return SRA;
	if (text == "syscall")
		return SYSCALL;

	throw std::invalid_argument("Unknown data.registers-type function code");
}

static r_types get_r_type(const uint8_t& opcode, const uint8_t& funct)
{
	if (opcode != 0)
	{
		throw std::invalid_argument("Not an data.registers-type instruction");
	}

	switch (funct)
	{
	case 0xC:
		return SYSCALL;
	case 0x20:
		return ADD;
	case 0x21:
		return ADDU;
	case 0x24:
		return AND;
	case 0x08:
		return JR;
	case 0x27:
		return NOR;
	case 0x25:
		return OR;
	case 0x2A:
		return SLT;
	case 0x2B:
		return SLTU;
	case 0x00:
		return SLL;
	case 0x02:
		return SRL;
	case 0x22:
		return SUB;
	case 0x23:
		return SUBU;
	case 0x10:
		return MFHI;
	case 0x12:
		return MFLO;
	case 0x18:
		return MULT;
	case 0x19:
		return MULTU;
	case 0x03:
		return SRA;
	default:
		throw std::invalid_argument("Unknown data.registers-type function code");
	}
}

struct RInstruction : Instruction
{
	uint8_t opcode;
	uint8_t funct;
	uint8_t shamt;

	uint8_t rs;
	uint8_t rd;
	uint8_t rt;

	r_types r_type;
	bool is_signed;

	RInstruction(
		const uint8_t& opcode, const uint8_t& funct, const uint8_t& shamt, const uint8_t& rd, const uint8_t& rs, const uint8_t& rt
	) : opcode(opcode), funct(funct), shamt(shamt), rs(rs), rd(rd), rt(rt), r_type(get_r_type(opcode, funct)), is_signed(r_type < 13)
	{

	}

	virtual void execute(ProgramMemory& data) override
	{
		switch (r_type)
		{
		case SYSCALL:
			data.syscall();
			break;
		case AND:
			data.registers[rd] = data.registers[rs] & data.registers[rt];
			break;
		case JR:
			data.PC = reinterpret_cast<uint8_t*>(data.registers[rs]);
			break;
		case NOR:
			data.registers[rd] = ~(data.registers[rs] | data.registers[rt]);
			break;
		case OR:
			data.registers[rd] = data.registers[rs] | data.registers[rt];
			break;
		case ADD:
		case ADDU:
			data.registers[rd] = data.registers[rs] + data.registers[rt];
			break;
		case SUB:
		case SUBU:
			data.registers[rd] = data.registers[rs] - data.registers[rt];
			break;
		case SLT:
		case SLTU:
			data.registers[rd] = data.registers[rs] < data.registers[rt] ? 1 : 0;
			break;
		case SLL:
			data.registers[rd] = data.registers[rt] << shamt;
			break;
		case SRL:
			data.registers[rd] = data.registers[rt] >> shamt;
			break;
		case SRA:
			data.registers[rd] = static_cast<int8_t>(data.registers[rt]) >> shamt;
			break;
		case MULT:
		{
			const int64_t total = static_cast<int64_t>(data.registers[rs]) * data.registers[rt];

			data.r_HI = total >> 32; // Isolate last 32bits
			data.r_LO = total & 0xFFFF'FFFF; // Isolate first 32bits

			break;
		}
		case MULTU:
		{
			const uint64_t total = static_cast<uint64_t>(data.registers[rs]) * data.registers[rt];

			data.r_HI = total >> 32; // Isolate last 32bits
			data.r_LO = total & 0xFFFF'FFFF; // Isolate first 32bits

			break;
		}
		case MFHI:
			data.registers[rd] = data.r_HI;
			break;
		case MFLO:
			data.registers[rd] = data.r_LO;
			break;
		}
	}
};