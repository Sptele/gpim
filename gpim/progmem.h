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