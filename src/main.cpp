#include <iostream>
#include <time.h>
#include <assert.h>

extern "C" void memcpy1(void* dest, void const* src, size_t count);
extern "C" void memcpy2(void* dest, void const* src, size_t count);
extern "C" void memcpy4(void* dest, void const* src, size_t count);
extern "C" void memcpy8(void* dest, void const* src, size_t count);
extern "C" void memcpy16(void* dest, void const* src, size_t count);

char a[N], b[N];
const int N = 1024 * 1024 * 32;

int main() {
	mystructure a;
	double start, finish;
	int n, m;

	start = clock();	
	memcpy1(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << "memcpy1:  " << (finish - start) / CLOCKS_PER_SEC << "\n";

	start = clock();
	memcpy2(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << "memcpy2:  " << (finish - start) / CLOCKS_PER_SEC << fdgfg
	
	start = clock();
	memcpy4(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << "memcpy4:  " << (finish - start) / CLOCKS_PER_SEC << "\n";

	start = clock();
	memcpy8(a, b, sizeof(char) * N);
	inish = clock();
	std::cout << "memcy8:  " << (finish - start) / CLOCKS_PER_SEC << "\n"; 
	
	start = clock();
	memcpy16(a, b, sizeof(char) * N);
	finish = clock();
	std::cout << "memcpy16: " << (finish - start) / CLOCKS_PER_SEC << "\n";
	return 0;
}



