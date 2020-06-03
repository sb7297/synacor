#include <iostream>
#include <fstream>
#include <array>
#include <cstdint>

using namespace std;

int main() {
	ifstream f("challenge.bin");
	array<char, 32768*2> memblock;
	array<uint16_t, 32768> decs;
	f.read(memblock.data(), memblock.size());
	for (int i = 0; i < decs.size(); i += 2) {
		/* decs[i] = ((uint16_t*) memblock.data())[i]; */
		decs[i/2] = memblock[i] + (memblock[i+1] << 8);
		cout << std::hex << i/2 << "L" << " " << decs[i/2] << "\n";
	}
	return 0;
}

