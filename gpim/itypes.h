#pragma once
#include <cstdint>
#include <stdexcept>

#include "instrtype.h"

enum i_types : uint8_t // may want to add l/s bytes
{
	ADDI,
	ANDI,
	BEQ,
	BNE,
	LW,
	ORI,
	SLTI,
	SW,
	ADDIU,
	SLTIU
};

static i_types get_i_type(const uint8_t& opcode)
{
	switch (opcode)
	{
	case 0x8: return ADDI;
	case 0x9: return ADDIU;
	case 0xC: return ANDI;
	case 0x4: return BEQ;
	case 0x5: return BNE;
	case 0x23: return LW;
	case 0xD: return ORI;
	case 0xA: return SLTI;
	case 0xB: return SLTIU;
	case 0x2B: return SW;
	default:
		if (opcode != 0x2 && opcode != 0x3)
			throw std::invalid_argument("Unknown I-type function code");
	}
}

struct IInstruction : Instruction
{
	uint8_t opcode;

	uint8_t rs;
	uint8_t rt;

	uint16_t imm;

	i_types i_type;
	bool is_signed;

	IInstruction(
		const uint8_t& opcode, const uint8_t& rs, const uint8_t& rt, const uint16_t& imm
	) : opcode(opcode), rs(rs), rt(rt), imm(imm), i_type(get_i_type(opcode)), is_signed(i_type < 8)
	{
		
	}

	virtual void execute(ProgramMemory& data) override
	{
		switch (i_type)
		{
		case ADDIU:
		case ADDI:
			data.registers[rt] = data.registers[rs] + imm;
			break;
		case ANDI:
			data.registers[rt] = data.registers[rs] & imm;
			break;
		case BEQ:
			{
				if (data.registers[rs] == data.registers[rt])
				{
					// offset is given in words, so we 
					data.PC += (static_cast<int16_t>(imm) * 4) + 4;
				}
			}
			break;
		case BNE:
			{
				if (data.registers[rs] != data.registers[rt])
				{
					// modify PC
					data.PC += (static_cast<int16_t>(imm) * 4) + 4;
				}
			}
			break;
		case LW:
			// Modify data memory
			data.registers[rt] = data.ram.read(data.registers[rs] + static_cast<int16_t>(imm));
			break;
		case SW:
			// Modify data memory
			data.ram.write(data.registers[rs] + static_cast<int16_t>(imm), data.registers[rt]);

			break;
		case ORI:
			data.registers[rt] = data.registers[rs] | imm;
			break;
		case SLTIU:
		case SLTI:
			data.registers[rt] = data.registers[rs] < imm ? 1 : 0;
		}
	}
};
