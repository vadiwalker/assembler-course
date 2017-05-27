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
extern "C" void memcpy16nt(void* dest, void const* src, size_t count);

void handle_memcpy(void* dest, void const* src, size_t count) {
	char* c_dest = static_cast<char*>(dest);
	char const* c_src = static_cast<char const*>(src);
	while (count--) {
		*(c_dest++) = *(c_src++);
	}
}

void memcpy1_impl(void* dest, void const* src, size_t count) {
	memcpy1(dest, src, count);
}

void memcpy4_impl(void* dest, void const* src, size_t count) {
	while (count && (intptr_t) dest % 4 != 0) {
		memcpy1(dest, src, 1);
		dest++;
		src++;
		count--;
	}
	memcpy4(dest, src, count);
}

void memcpy8_impl(void* dest, void const* src, size_t count) {
	while (count && (intptr_t) dest % 8 != 0) {
		memcpy1(dest, src, 1);
		dest++;
		src++;
		count--;
	}
	memcpy8(dest, src, count);
}

void memcpy16_impl(void* dest, void const* src, size_t count) {
	while (count && (intptr_t) dest % 16 != 0) {
		memcpy1(dest, src, 1);
		dest++;
		src++;
		count--;
	}
	memcpy16(dest, src, count);
}

void memcpy16nt_impl(void* dest, void const* src, size_t count) {
    while (count && (intptr_t) dest % 16 != 0) {
        memcpy1(dest, src, 1);
        dest++;
        src++;
        count--;
    }
    memcpy16nt(dest, src, count);
}

const int N = 1024 * 1024 * 124;
char a[N], b[N];

int main() {
	double start, finish;

	std::cout << std::fixed << std::setprecision(6);

	start = clock();
	memcpy1_impl(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << (finish - start) / CLOCKS_PER_SEC << " - memcpy1\n";

	start = clock();
	handle_memcpy(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << (finish - start) / CLOCKS_PER_SEC << " - memcpy for loop\n";

	start = clock();
	memcpy4_impl(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << (finish - start) / CLOCKS_PER_SEC << " - memcpy4\n";

	start = clock();
	memcpy8_impl(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << (finish - start) / CLOCKS_PER_SEC << " - memcpy8\n";

	start = clock();
	memcpy16_impl(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << (finish - start) / CLOCKS_PER_SEC << " - memcpy16\n";	

    start = clock();
    memcpy16nt_impl(a, b, sizeof(char) * N);
    finish = clock();
    std::cout << (finish - start) / CLOCKS_PER_SEC << " - memcpy16nt\n";

    start = clock();
	memcpy(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << (finish - start) / CLOCKS_PER_SEC << " - memcpy built-in\n";

    return 0;
}