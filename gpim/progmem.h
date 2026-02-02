#pragma once

#include <cstdint>

struct DataWriter
{
	// TODO: read & write function to interact with data.bin

	std::string file_path;

	DataWriter() : file_path("data.bin") {}
	DataWriter(const std::string& file_path) : file_path(file_path) {}
};


struct ProgramMemory
{
	uint32_t PC = 0;
	uint32_t registers[32] = {};
	uint32_t r_HI = 0;
	uint32_t r_LO = 0;
};