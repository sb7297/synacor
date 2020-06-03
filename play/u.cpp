#include <cstdint>
#include <iostream>

using namespace std;

union Number {
	int16_t num;
	uint8_t bytes[2];
};

int main() {
	Number n;
	n.bytes[0] = 0x15; n.bytes[1] = 0x00;
	cout << n.num << " " << n.bytes[0] << " " << n.bytes[1] << "\n";
	return 0;
}
