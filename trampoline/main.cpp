#include "trampoline.h"
#include <iostream>
#include <cstdio>
#include <vector>
#include <cassert>

struct sum {
    int operator()(int c1, int c2, int c3, int c4, int c5) {
        return c1 + c2 + c3 + c4 + c5;
    }
};


void test1() {
    sum sum_object;
    trampoline<int (int, int, int, int, int)> tr(sum_object);
    auto ptr = tr.get();
    int a = rand() % 1000;
    int b = rand() % 1000;
    int c = rand() % 1000;
    int d = rand() % 1000;
    int e = rand() % 1000;

    int expected_ret = a + b + c + d + e;
    assert(expected_ret == ptr(a, b, c, d, e));
}

void test2() {
    int a1 = rand();
    int a2 = rand();
    int a3 = rand();
    int a4 = rand();
    int a5 = rand();
    int a6 = rand();
    int a7 = rand();
    int a8 = rand();
    int a9 = rand();
    trampoline<long long (int, int, int, int, int, int, int, int, int)>
            tr([&](int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9) {
                   return (long long) a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9;
               });
    auto ptr = tr.get();
    long long expected_ret = (long long) a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9;
    assert(expected_ret == ptr(a1, a2, a3, a4, a5, a6, a7, a8, a9));
}

int main() {
    test1();
    test2();
    std::cout << "Tests passed.\n";
    return 0;
}