#pragma once

#include <cstdint>

struct Instruction
{
	virtual ~Instruction() = default;
	virtual void execute(uint32_t* R, uint32_t& HI, uint32_t& LO, uint32_t& PC) = 0;

};
