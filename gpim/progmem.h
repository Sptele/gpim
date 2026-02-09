#pragma once

#include <cstdint>

struct DataManager
{
	// TODO: read & write function to interact with data.bin

	std::string file_path;

	DataManager() : file_path("data.bin")
	{
	}

	DataManager(const std::string& file_path) : file_path(file_path)
	{
	}
};


struct ProgramMemory
{
	uint8_t* PC;
	uint32_t registers[32];
	uint32_t r_HI;
	uint32_t r_LO;
	uint8_t* buffer;

	ProgramMemory() : PC(nullptr), registers{}, r_HI(0), r_LO(0), buffer()
	{
	}

	ProgramMemory(uint8_t* buffer, int len) : ProgramMemory()
	{
		this->buffer = buffer;

		// TODO: instead of just swapping / reinterpret, we can build the PC from the buffer
		// TODO: then test that PC++ will do the proper endian
		// TODO: if not, then create a function for the PC

		PC = this->buffer;
	}

	uint8_t* operator++(int)
	{
		uint8_t* rtnr = PC;

		PC += 4;

		return rtnr;
	}

	uint32_t operator*()
	{
		uint8_t* temp = PC;

		return (*temp++ << 24) + (*temp++ << 16) + (*temp++ << 8) + *temp;
	}

	uint8_t* operator+(const int& o)
	{
		uint8_t* temp = PC;

		return temp + o;
	}
};
