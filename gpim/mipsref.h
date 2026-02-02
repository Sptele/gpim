#pragma once

#include <cstdint>
#include <sstream>
#include <memory>

#include "rtypes.h"
#include "itypes.h"
#include "jtypes.h"

static std::unique_ptr<Instruction> interpret(const uint32_t& bits)
{
	uint8_t opcode = bits >> 26 & 0x3F;

	switch (opcode)
	{
	case 0: // R-Type
		return std::make_unique<RInstruction>(RInstruction(
			opcode,
			bits & 0x3F,
			bits >> 6 & 0x1F,
			bits >> 11 & 0x1F,
			bits >> 21 & 0x1F,
			bits >> 16 & 0x1F
		));
	case 0x2: // J-Type
	case 0x3:
		return std::make_unique<JInstruction>(JInstruction(
			opcode,
			bits & 0x03FF'FFFF
		));
	default: // I-Type
		return std::make_unique<IInstruction>(IInstruction(
			opcode,
			bits >> 21 & 0x1F,
			bits >> 16 & 0x1F,
			bits & 0xFFFF
		));
	}
}


static std::string get_register_name(const uint8_t& r_num)
{
	std::ostringstream rtnr;

	rtnr << "$";

	if (r_num == 0) rtnr << "zero";
	else if (r_num == 1) rtnr << "at";
	else if (2 <= r_num && r_num <= 3) rtnr << "v" << r_num - 2;
	else if (4 <= r_num && r_num <= 7) rtnr << "a" << r_num - 4;
	else if (8 <= r_num && r_num <= 15) rtnr << "t" << r_num - 8;
	else if (16 <= r_num && r_num <= 23) rtnr << "s" << r_num - 16;
	else if (24 <= r_num && r_num <= 25) rtnr << "t" << r_num - 16;
	else if (26 <= r_num && r_num <= 27) rtnr << "k" << r_num - 26;
	else if (r_num == 28) rtnr << "gp";
	else if (r_num == 29) rtnr << "sp";
	else if (r_num == 30) rtnr << "fp";
	else if (r_num == 31) rtnr << "ra";

	return rtnr.str();
}

static uint8_t get_register_index(const std::string& r_name)
{
	if (r_name == "")
		throw std::invalid_argument("Empty register name!");

	if (r_name == "$zero")
		return 0;
	if (r_name == "$at")
		return 1;
	if (r_name == "$gp")
		return 28;
	if (r_name == "$sp")
		return 29;
	if (r_name == "$fp")
		return 30;
	if (r_name == "$ra")
		return 31;

	uint8_t r_index = r_name[2] - '0';

	if (r_name[1] == 'v')
		return r_index + 2;
	if (r_name[1] == 'a')
		return r_index + 4;
	if (r_name[1] == 't')
		return r_index + (r_index <= 7 ? 8 : 16);
	if (r_name[1] == 's')
		return r_index + 16;
	if (r_name[1] == 'k')
		return r_index + 26;
	
	return static_cast<uint8_t>(-1);
}
