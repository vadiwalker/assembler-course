#include <iostream>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <iomanip>
#include <bits/stdc++.h>

extern "C" void memcpy1(void* dest, void const* src, size_t count);
extern "C" void memcpy4(void* dest, void const* src, size_t count);
extern "C" void memcpy8(void* dest, void const* src, size_t count);
extern "C" void memcpy16(void* dest, void const* src, size_t count);

void handle_memcpy(void* dest, void const* src, size_t count) {
	char* c_dest = static_cast<char*>(dest);
	char const* c_src = static_cast<char const*>(src);
	while (count--) {
		*(c_dest++) = *(c_src++);
	}
}


const int N = 1024 * 1024 * 124;
char a[N], b[N];

int main() {
	double start, finish;

	std::cout << std::fixed << std::setprecision(6);

	start = clock();
	memcpy1(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << (finish - start) / CLOCKS_PER_SEC << " - memcpy1\n";

	start = clock();
	handle_memcpy(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << (finish - start) / CLOCKS_PER_SEC << " - memcpy for loop\n";

	start = clock();
	memcpy4(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << (finish - start) / CLOCKS_PER_SEC << " - memcpy4\n";

	start = clock();
	memcpy8(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << (finish - start) / CLOCKS_PER_SEC << " - memcpy8\n";

	start = clock();
	memcpy16(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << (finish - start) / CLOCKS_PER_SEC << " - memcpy16\n";	

	start = clock();
	memcpy(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << (finish - start) / CLOCKS_PER_SEC << " - memcpy built-in\n";
	return 0;
}