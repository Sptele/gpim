#pragma once

#include <cstdint>

struct DataManager
{
	// TODO: read & write function to interact with data.bin

	std::string file_path;

	DataManager() : file_path("data.bin") {}
	DataManager(const std::string& file_path) : file_path(file_path) {}
};


struct ProgramMemory
{
	uint32_t PC = 0; // TODO: PC should be a pointer (uint32_t*) that literally stores the address of some word
	uint32_t registers[32] = {};
	uint32_t r_HI = 0;
	uint32_t r_LO = 0;
};