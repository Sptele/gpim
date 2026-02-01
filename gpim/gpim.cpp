// gpim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <bitset>
#include <iomanip>
#include <string>
#include <fstream>

#include "mipsref.h"


void cycle(const uint32_t& bits, uint32_t* R, uint32_t& HI, uint32_t& LO, uint32_t& PC)
{
	auto instr = interpret(bits);
	instr->execute(R, HI, LO, PC);
}

void print_state(const uint32_t& bits, uint32_t* R, uint32_t& HI, uint32_t& LO, uint32_t& PC)
{
	//RInstruction instr = interpret(bits);

	//std::cout << "Current Instruction: " << std::endl;
	//std::cout << "  >> opcode: " << static_cast<uint32_t>(instr.opcode) << std::endl;
	//std::cout << "  >> funct: " << std::hex << static_cast<uint32_t>(instr.funct) << std::dec << std::endl;
	//std::cout << "  >> shamt: " << static_cast<uint32_t>(instr.shamt) << std::endl;
	//std::cout << "  >> rs: " << static_cast<uint32_t>(instr.rs) << std::endl;
	//std::cout << "  >> rd: " << static_cast<uint32_t>(instr.rd) << std::endl;
	//std::cout << "  >> rt: " << static_cast<uint32_t>(instr.rt) << std::endl;
	//std::cout << "  >> r_type: " << static_cast<uint32_t>(instr.r_type) << std::endl;
	//std::cout << "  >> is_signed: " << instr.is_signed << std::endl;
	//std::cout << std::endl;

	std::cout << "Registers: " << std::endl;
	 
	for (int i = 0; i < 32; ++i)
	{
		std::cout << "  >> r" << i << ": ";

		// Ternaries must resolve to the same type
		// Thus, leaving the else block of this ternary as a s_cast to uint32_t would result in the entire ternary resolving
		// to uint32_t. Implicitly, the if block would also be casted!
		// So, to make sure this ternary always resolves to a signed integer, we cast to and from uint32_t
		// Casting from uint32_t will not flip the sign back

		// This logic above forgets about overflow! When we cast back to int32_t, our uint may be above 2^31-1, so it'll go back to a negative
		std::cout << R[i] << std::endl;
	}


	std::cout << std::endl;
	std::cout << "HI: " << HI << std::endl;
	std::cout << "LO: " << LO << std::endl;

	std::cout << "PC: " << PC << std::endl;
}

uint32_t compile(const std::string& line)
{
	// split string by spaces (exclude commas)
	std::string commands[4];

	uint8_t arr_i = 0;
	uint8_t curr_word_i = 0;
	uint8_t curr_len = 0;
	for (size_t i = 0; i < line.length() + 1; ++i)
	{
		if (line[i] != ',')
		{
			if (i == line.length() || line[i] == ' ')
			{
				commands[arr_i] = line.substr(curr_word_i, curr_len);
				curr_word_i = i + 1;
				curr_len = 0;

				++arr_i;
			}
			else ++curr_len;
		}
	}

	// For each part of the r-type, identify: command, registers/shamt

	r_types type = get_r_type(commands[0]);
	auto [opcode, funct] = get_opcode_funct(type);

	// Now read the three registers

	// first = rd
	// second = rs
	// third = rt

	bool req_shamt = type == SLL || type == SRL || type == SRA;

	uint8_t rd = get_register_index(commands[1]);
	uint8_t rs = get_register_index(commands[2]);
	uint8_t rt = req_shamt ? std::stoul(commands[3]) : get_register_index(commands[3]);

	// Time to encode!

	uint32_t encoded = 0;

	encoded += opcode << 26;
	encoded += rs << 21;
	encoded += req_shamt ? 0 : rt << 16;
	encoded += rd << 11;
	encoded += req_shamt ? rt << 6 : 0;
	encoded += funct;

	return encoded;
}

void load_file(const std::string& path, char* buffer, size_t buffer_length)
{
	// Let's just pretend for now that we
	std::ifstream inp(path, std::ios::in | std::ios::binary);

	if (!inp.is_open())
	{
		std::cerr << "Failed to open the file!" << std::endl;
		return;
	}

	inp.read(buffer, buffer_length * sizeof(char));

	for (size_t i = 0; i < buffer_length; ++i)
	{
		std::cout << buffer[i] << " ";
	}

	// TODO: pasting into .bin pastes it as an ASCII strng, not as hex
	// So, directly paste the hex into your bin file and then open it
	// Here: 2108000A2129001401095020

	inp.close();
}

// ADD $t0, $t1, $t2 : 0x012A4020 20 40
// ADDI $t0, $t1, 2 : 0x21280002 20 0 -> 22
int main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cerr << "Usage: " << argv[0] << " <path> <file_length>" << std::endl;
		return 1;
	}

	std::string inpf = argv[1];
	int file_length = std::stoi(argv[2]);
	char* buffer = new char[file_length];

	load_file(inpf, buffer, file_length);

	uint32_t PC = 0;

	// Buffer now stores the input, byte-addressable
	// Thus, the PC needs to simply store an index
	// - to hit words, it needs to be in multiples of 4


	// TODO:

	uint32_t bits = 0;
	

	std::cout << "Instruction bits: " << std::bitset<32>(bits) << " (0x" << std::hex << std::setfill('0') << bits <<
		std::dec << ")" << std::endl;

	uint32_t registers[32] = {};
	uint32_t HI_register = 0;
	uint32_t LO_register = 0;

	cycle(bits, registers, HI_register, LO_register, PC);

	print_state(bits, registers, HI_register, LO_register, PC);

	delete[] buffer;
}
