#include <iostream>
#include <array>
#include <algorithm>

using namespace std;

int main() {
	array<int, 5> x = {2, 3, 5, 7, 9};

	do {
		if (x[0] + x[1] * (x[2]*x[2]) + (x[3]*x[3]*x[3]) - x[4] == 399) {
			for (auto i : x) {
				cout << i << " ";
			} cout << "\n";
		}
	} while(next_permutation(x.begin(), x.end()));
	return 0;
}
