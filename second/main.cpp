#include <iostream>

using namespace std;

extern "C" int count_words(char const* s);

extern "C" int md(int a, int b) {
	
}

int main() {


	std::cout << (int) (' ') << "\n";
	char const* s = "101010";
	std::cout << count_words(s) << "\n";
	return 0;
}