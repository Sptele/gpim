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
	uint32_t* PC;
	uint32_t registers[32];
	uint32_t r_HI;
	uint32_t r_LO;

	ProgramMemory() : PC(nullptr), registers{}, r_HI(0), r_LO(0)
	{
		
	}

	ProgramMemory(uint8_t* buffer, int len) : ProgramMemory()
	{

		// Reverese every four bytes
		if (!buffer || len <= 0)
		{
			PC = reinterpret_cast<uint32_t*>(buffer);
			return;
		}

		// Reverse every consecutive 4-byte group in-place.
		const int groups = len / 4;
		for (int g = 0; g < groups; ++g)
		{
			uint8_t* b = buffer + g * 4;
			std::swap(b[0], b[3]);
			std::swap(b[1], b[2]);
		}

		PC = reinterpret_cast<uint32_t*>(buffer);
	}

};