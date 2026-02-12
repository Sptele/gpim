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

		std::fstream out(file_path, std::ios::in | std::ios::out | std::ios::binary);

		if (!out.is_open()) return;

		char buffer[4] = {
			static_cast<char>(data >> 24 & 0xFF),
			static_cast<char>(data >> 16 & 0xFF),
			static_cast<char>(data >> 8 & 0xFF),
			static_cast<char>(data & 0xFF)
		};

		out.seekp(addr, std::ios::beg);
		out.write(buffer, 4); // may need to write to a buffer
		out.close();


	}

	uint32_t read(const uint32_t& addr) const {
		std::ifstream in(file_path, std::ios::in | std::ios::binary);

		if (!in.is_open()) return -1; // int max

		in.seekg(addr, std::ios::beg);

		char buffer[4] = {};

		in.read(buffer, 4);
		in.close();

		return static_cast<uint32_t>(
			(buffer[0] << 24) +
			(buffer[1] << 16) +
			(buffer[2] << 8) +
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

	bool debug;



	DataManager ram;

	ProgramMemory() : PC(nullptr), registers{}, r_HI(0), r_LO(0), buffer(), debug(false)
	{
	}

	ProgramMemory(uint8_t* buffer, bool debug) : ProgramMemory()
	{
		this->buffer = buffer;
		this->debug = debug;

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

	void syscall()
	{
		const int V0 = 2;
		const int A0 = 4;
		switch (V0)
		{
		case 1:
			std::cout << registers[A0];
			break;
		case 4:
			// No-op
			break;
		case 5:
			std::cin >> registers[V0];
			break;
		case 10:
			exit(0);
		default:
			// no-op, not implemented
			if (debug) std::cout << "[gpim] syscall code " << registers[V0] << " not implemented! (no-op)" << std::endl;
		}
	}

};
