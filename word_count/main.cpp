#include <iostream>
#include <string.h>
#include <emmintrin.h>
#include <stdio.h>
#include <tmmintrin.h>
#include <vector>
#include <assert.h>

int trivial_count_words(std::string const& s) {
    int ret = 0;
    if (s[0] != ' ')
        ret = 1;
    for (int i = 1; i < (int) s.size(); i++) {
        if (s[i] != ' ' && s[i - 1] == ' ') ret++;
    }
    return ret;
}

int count_first_symbs(char const* s, size_t sz) {
    int ret = 0;
    for (int i = 0; i < sz; i++) {
        if (s[i] != ' ' && (i == 0 || s[i - 1] == ' ')) ret++;
        if ((size_t) (s + i) % 16 == 0) break; 
    }
    return ret;
}

int count_last_symbs(char const* s, int size) {
    int ret = 0;
    size_t pos = size - size % 16;
    if(pos > 0 && s[pos - 1] == ' ' && s[pos] != ' ') ret--;
    for (int i = pos; i < size; i++) {
        if (i > 0 && s[i] != ' ' && s[i - 1] == ' ') ret++;
    }
    return ret;
}

int count_middle_symbs(char const* c, int size) {
    int ret = 0;

    __m128i cmp, cmp_sh, tmp;
    __m128i store = _mm_set_epi32(0, 0, 0, 0);
    __m128i space = _mm_set_epi8(32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32);
    __asm__("movdqa\t" "(%2), %1\n"
            "pcmpeqb\t" "%1, %0"
            :"=x"(cmp_sh), "=x"(tmp)
            :"r"(c), "0"(space));
    
    size_t n = size / 16;
    for (size_t i = 0; i < n; i++) {
        
        cmp = cmp_sh;

        uint32_t rt = 0;
        
        __m128i x1, x2, x3, x4;

        __asm__("movdqa\t" "(%7), %3\n"
                "pcmpeqb\t" "%3, %0\n"
                "movdqa\t" "%0, %6\n"
                "palignr\t" "$1, %4, %6\n"
                "pandn\t" "%4, %6\n"
                "psubsb\t" "%6, %5\n"
                "paddusb\t" "%5, %1\n"
                "pmovmskb\t" "%1, %2"
                :"=x"(cmp_sh), "=x"(store), "=r"(rt), "=x"(x1), "=x"(x2), "=x"(x3), "=x"(x4)
                :"r"(c + 16 * (i + 1)), "0"(space), "4"(cmp), "5"(_mm_set_epi32(0, 0, 0, 0)), "1"(store));
        
        if (rt) {
            uint32_t high, low;
            __m128i result_of_abs, abs_low, tt;

            __asm__("psadbw\t" "%3, %0\n"
                    "movd\t"   "%0, %2\n"
                    "movhlps\t" "%0, %0\n"
                    "movd\t" "%0, %1\n"
                    :"=x"(tt), "=r"(high), "=r"(low), "=x"(abs_low)
                    :"0"(_mm_set_epi32(0, 0, 0, 0)), "3"(store)
                    :"0");
            
            ret += high + low;
            store = _mm_set_epi32(0, 0, 0, 0);
        }
    }
    
    __m128i result_of_abs, abs_low, tt;
    uint32_t high, low;
    
    __asm__("psadbw\t" "%3, %0\n"
            "movd\t"   "%0, %2\n"
            "movhlps\t" "%0, %0\n"
            "movd\t" "%0, %1\n"
            :"=x" (tt), "=r"(high), "=r"(low), "=x"(abs_low)
            :"0"(_mm_set_epi32(0, 0, 0, 0)), "3"(store)
            :"0");
    
    ret += high + low;
    store = _mm_set_epi32(0, 0, 0, 0);

    return ret;
}

int count_words(std::string const& s) {
    int ret = 0;
    char const *c = s.c_str();
    size_t size = s.size();

    ret += count_first_symbs(c, size);
    while ((size_t) c % 16 != 0 && size > 0) {
        c++;
        size--;
    }
    ret += count_last_symbs(c, size);
    ret += count_middle_symbs(c, size);
    return ret;
}

void test() {
    std::string test_string = "pro fessionals cat     dog hot cccc aaa    abacaba home in the university bibip yes no maybe  lets   go";
    for (int spaces_at_begin = 0; spaces_at_begin < 20; spaces_at_begin++) {
        for (int spaces_at_end = 0; spaces_at_end < 20; spaces_at_end++) {
            std::string before(spaces_at_begin, ' ');
            std::string after(spaces_at_end, ' ');
            std::string cur = before + test_string + after;
            if (trivial_count_words(cur) != count_words(cur)) {
                std::cout << "Tests failed on test:\n" << cur << "\n";
                return;
            }
        }
    }
    std::cout << "Tests passed successfully\n";
}

int main() {
    test();
    return 0;
}