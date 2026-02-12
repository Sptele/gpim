// gpim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <bitset>
#include <iomanip>
#include <string>
#include <fstream>

#include "mipsref.h"

namespace
{
	void zero(ProgramMemory& data)
	{
		// $zero must stay $zero
		data.registers[0] = 0;
	}

	void cycle(const uint32_t& bits, ProgramMemory& data)
	{
		zero(data);

		auto instr = interpret(bits);
		instr->execute(data);

		zero(data);
	}

	static void print_state(const uint32_t& instruction, uint32_t* R, uint32_t& HI, uint32_t& LO, uint8_t* PC)
	{
		std::cout << "[PC=" << PC << "]->{ ";
		for (int i = 24; i >= 0; i -= 8)
		{
			std::cout << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << (instruction >> i & 0xFF) <<
				" ";
		}

		std::cout << std::dec << "}" << std::endl;

		std::cout << "Active Registers:" << std::endl;

		for (size_t i = 0; i < 32; ++i)
		{
			if (R[i] == 0) continue;

			std::cout << "[" << i << "]->{" << R[i] << "}" << std::endl;
		}

		std::cout << "--[HI,LO]->{" << HI << "," << LO << "}" << std::endl << std::endl;
	}

	void load_file(const std::string& path, uint8_t* buffer, size_t buffer_length)
	{
		std::ifstream inp(path, std::ios::in | std::ios::binary);

		if (!inp.is_open())
		{
			std::cerr << "Failed to load instructions!" << std::endl;
			return;
		}

		inp.read(reinterpret_cast<char*>(buffer), buffer_length * sizeof(char));

		std::cout << "Buffer State: ";

		for (size_t i = 0; i < buffer_length; ++i)
		{
			std::cout << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << (int)buffer[i] << " ";
		}

		std::cout << std::dec << std::endl;

		inp.close();
	}

	int get_file_size(const std::string& path)
	{
		std::ifstream inp(path, std::ios::in | std::ios::binary | std::ios::ate);

		if (!inp.is_open())
		{
			std::cerr << "Failed to load instructions!" << std::endl;
			exit(1);
		}

		std::streampos size = inp.tellg();
		inp.close();

		if (size == -1)
		{
			std::cerr << "Error reading file size!" << std::endl;
			exit(1);
		}

		return size;
	}

	bool is_safe_mode(const int& argc, char** argv)
	{
		for (int i = 1; i < argc; ++i)
		{
			if (strcmp(argv[i], "-sm") == 0 || strcmp(argv[i], "--safe-mode") == 0 || strcmp(argv[i], "--safe") == 0)
				return true;
		}

		return false;
	}

	bool is_print_mode(const int& argc, char** argv)
	{
		for (int i = 1; i < argc; ++i)
		{
			if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--print") == 0 || strcmp(argv[i], "--debug") == 0 ||
				strcmp(argv[i], "--print-mode") == 0 || strcmp(argv[i], "-p") == 0)
				return true;
		}

		return false;
	}
}

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cerr << "Usage: " << argv[0] << " <path> <file_length>" << std::endl;
		return 1;
	}

	std::string inpf = argv[1];
	bool SAFE_MODE = is_safe_mode(argc, argv);
	bool PRINT_STATE = is_print_mode(argc, argv);

	int file_length_b = get_file_size(inpf);
	uint8_t* buffer = new uint8_t[file_length_b];

	load_file(inpf, buffer, file_length_b);

	ProgramMemory data(buffer, SAFE_MODE);

	while (true)
	{
		if (SAFE_MODE && data.PC >= buffer + file_length_b) break;

		uint32_t curr_bits = *data;

		data++;

		cycle(curr_bits, data);

		if (PRINT_STATE)
			print_state(curr_bits, data.registers, data.r_HI, data.r_LO, data.PC);
	}

	// Need to free buffer
	delete[] buffer;
}
