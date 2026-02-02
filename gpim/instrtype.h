#pragma once

#include <cstdint>

#include "progmem.h"

struct Instruction
{
	virtual ~Instruction() = default;
	virtual void execute(ProgramMemory& data) = 0;

};
