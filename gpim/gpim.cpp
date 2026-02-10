// gpim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <bitset>
#include <iomanip>
#include <string>
#include <fstream>

#include "mipsref.h"

#define PRINT_STATE 1


void cycle(const uint32_t& bits, ProgramMemory& data)
{
	auto instr = interpret(bits);
	instr->execute(data);
}

void print_state(const uint32_t& instruction, uint32_t* R, uint32_t& HI, uint32_t& LO, uint8_t* PC)
{
	std::cout << "[PC=" << PC << "]->{ ";
	for (int i = 24; i >= 0; i -= 8)
	{
		std::cout << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << (instruction >> i & 0xFF) << " ";
	}

	std::cout << std::dec << "}" << std::endl;

	std::cout << "Active Registers:" << std::endl;

	for (size_t i = 0; i < 32; ++i) {
		if (R[i] == 0) continue;

		std::cout << "[" << i << "]->{" << R[i] << "}" << std::endl;
	}

	std::cout << "--[HI,LO]->{" << HI << "," << LO << "}" << std::endl << std::endl;
}

void load_file(const std::string& path, uint8_t* buffer, size_t buffer_length)
{
	// Let's just pretend for now that we
	std::ifstream inp(path, std::ios::in | std::ios::binary);

	if (!inp.is_open())
	{
		std::cerr << "Failed to open the file!" << std::endl;
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

/*
-- 2108000A2129001401095020
addi $t0, $t0, 10
addi $t1, $t1, 20
add $t2, $t0, $t1
*/

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cerr << "Usage: " << argv[0] << " <path> <file_length>" << std::endl;
		return 1;
	}

	std::string inpf = argv[1];
	int file_length_b = std::stoi(argv[2]) * 4;
	uint8_t* buffer = new uint8_t[file_length_b];

	load_file(inpf, buffer, file_length_b);

	ProgramMemory data(buffer, file_length_b);	

	while (true) {
		if ((data.PC) >= buffer + file_length_b) break;

		uint32_t curr_bits = *data;

		data++;

		cycle(curr_bits, data);

#if PRINT_STATE
		print_state(curr_bits, data.registers, data.r_HI, data.r_LO, data.PC);
#endif
	}

	// Need to free buffer
	delete[] buffer;
}
