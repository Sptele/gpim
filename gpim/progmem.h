#pragma once

#include <fstream>
#include <cstdint>

struct DataManager
{
	// TODO: read & write function to interact with data.bin

	std::string file_path;

	DataManager() : file_path("data.bin") {}
	DataManager(const std::string& file_path) : file_path(file_path) {}

	void write(const uint32_t& addr, const uint32_t& data) const {
		// for data.bin

		std::ofstream out(file_path, std::ios::app | std::ios::binary);

		if (!out.is_open()) return; // TODO

		out.seekp(addr, std::ios::beg);
		out.write(reinterpret_cast<char*>(addr), 4); // may need to write to a buffer
		out.close();


	}

	uint32_t read(const uint32_t& addr) const {
		std::ifstream in(file_path, std::ios::in | std::ios::binary);

		if (!in.is_open()) return 0; // TODO

		in.seekg(addr, std::ios::beg);

		char buffer[4] = {};

		in.read(buffer, 4);
		in.close();

		return static_cast<uint32_t>(
			buffer[0] << 24 +
			buffer[1] << 16 +
			buffer[2] << 8 +
			buffer[3]
			);
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
