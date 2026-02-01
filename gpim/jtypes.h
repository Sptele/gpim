#pragma once
#include <cstdint>
#include <stdexcept>

#include "instrtype.h"

enum j_types : uint8_t
{
	J,
	JAL
};

static j_types get_j_type(const uint8_t& opcode)
{
	switch (opcode)
	{
	case 0x2: return J;
	case 0x3: return JAL;
	default:
		throw std::invalid_argument("Unknown J-type function code");
	}
}

struct JInstruction : Instruction
{
	uint8_t opcode;
	uint32_t address;

	j_types j_type;

	JInstruction(
		const uint8_t& opcode, const uint32_t& address
	) : opcode(opcode), address(address), j_type(get_j_type(opcode))
	{}

	virtual void execute(uint32_t* R, uint32_t& HI, uint32_t& LO, uint32_t& PC) override
	{
		switch (j_type)
		{
		case J:
			PC = address; // is this correct?
			break;
		case JAL:
			R[31] = PC + 8; //???
			PC = address; //???
		}
	}
};