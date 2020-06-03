#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <array>

using namespace std;

struct Machine {
	array<uint16_t, 32768> memory;
	array<uint16_t, 8> regs;
	vector<uint16_t> stack;
	uint16_t pc; // program counter
	Machine() : pc(0) {
		memory = {};
		regs = {}; // zero initialize ("value initialize") memory and regs
	}
	uint16_t read(uint16_t addr) {
		uint16_t val = memory.at(addr);
		if (val < 0x8000) return memory[addr];
		else if (val <= 0x8007) {
			return regs.at(val-0x8000);
		}
	}
	void write(uint16_t addr, uint16_t val) {
		regs[memory[addr]-0x8000] = val;
	}
	int step() {
		uint16_t op = memory[pc];
		switch(op) {
			case 0: return 0; // halt
			case 1: { // set
				write(pc+1, read(pc+2));
				pc += 3;
				return 1;
			}
			case 2: { // push
				stack.push_back(read(pc+1));
				pc += 2; return 2;
			}
			case 3: { // pop
				if (!stack.empty()) {
					auto val = stack[stack.size()-1];
					write(pc+1, val);
					stack.pop_back();
				} else { return -3; }
				pc += 2; return 3;
			}
			case 4 : { // eq
				write(pc+1, (read(pc+2) == read(pc+3)) ? 1 : 0);
				pc += 4; return 4;
			}
			case 5 : { // gt
				write(pc+1, (read(pc+2) > read(pc+3)) ? 1 : 0);
				pc += 4; return 5;
			}
			case 6: { // jmp
				pc = read(pc+1);
				return 6;
			}
			case 7: { // jt
				if (read(pc+1) != 0) {
					pc = read(pc+2);
					return 7;
				}
				pc += 3; return 7;
			}
			case 8: { // jf
				if (read(pc+1) == 0) {
					pc = read(pc+2);
					return 8;
				}
				pc += 3; return 8;
			}
			case 9: { // add
				write(pc+1, (read(pc+2) + read(pc+3)) % 0x8000);
				pc += 4; return 9;
			}
			case 12: { // and
				write(pc+1, read(pc+2) & read(pc+3));
				pc += 4; return 12;
			}
			case 13: { // or
				write(pc+1, read(pc+2) | read(pc+3));
				pc += 4; return 13;
			}
			case 14: { // not
				auto val = read(pc+2);
				val = ~val;
				// discard msb - we only want 15 bit integers
				val = val << 1;
				val = val >> 1;
				write(pc+1, val);
				pc += 3; return 14;
			}
			case 19: { // out
				cout << (char) read(pc+1);
				pc += 2;
				return 19;
			}
			case 21: { // nop
				pc += 1;
				return 21;
			}
		}
		return -op;
	}
};

int main() {
	Machine machine;
	{
		ifstream f("challenge.bin");
		array<char, 32768*2> memblock;
		f.read(memblock.data(), memblock.size());
		for (int i = 0; i < machine.memory.size(); i += 1) {
			// v Mysterious off-by-one on higher order byte
			/* machine.memory.m[i/2].n = memblock[i] + (memblock[i+1] << 8); */
			machine.memory[i] = ((uint16_t*) memblock.data())[i];
		}
	}

	while (1) {
		int ret = machine.step();
		if (ret == 0) {
			cout << std::hex << "!! Halt encountered at address " << machine.pc << " !!\n";
			break;
		}
		if (ret == -3) {
			cout << std::hex << "tried to pop from empty stack at " << machine.pc << "\n";
			break;
		}
		if (ret < 0) {
			cout << "!! Unimplemented op " << (-ret) << " encountered at address "
				<< std::hex << "0x" << machine.pc << " !!\n";
			break;
		}
	}

	return 0;
}
