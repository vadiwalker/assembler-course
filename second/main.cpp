#include <bits/stdc++.h>

int trivial_count(std::string const& s) {
    int ret = 0;
    for (int i = 0; i < (int) s.size(); i++) {
    	if (s[i] != ' ' && (!i || s[i - 1] == ' '))
    		ret++;
    }
    return ret;
}

int count(std::string const& s) {
	if (s.size() < 64) {
		return trivial_count(s);
	}

    char const *c = s.c_str();
    assert((long long) c % 16 == 0);
    size_t sz = s.size();
    int ret = 0;

    ret += (c[0] != ' ');

    __m128i tmp;
    __m128i whitespace_reg =
    _mm_set_epi8(32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32);
    __m128i cmp_ret, cmp_ret_sh;
    __m128i store = _mm_set_epi32(0, 0, 0, 0);
    __m128i abs_ret, abs_low, tt;
    uint32_t high, low;

    __asm__("movdqa\t" "(%2), %1\n"
            "pcmpeqb\t" "%1, %0"
            :"=x"(cmp_ret_sh), "=x"(tmp)
            :"r"(c), "0"(whitespace_reg));
    
    int n = sz / 16;
    for (size_t i = 0; i < n; i++) {
        
        cmp_ret = cmp_ret_sh;
        uint32_t cur_ret;
        
        __m128i a1, a2, a3, a4;

        __asm__("movdqa\t" "(%7), %3\n"
                "pcmpeqb\t" "%3, %0\n"
                "movdqa\t" "%0, %6\n"
                "palignr\t" "$1, %4, %6\n"
                "pandn\t" "%4, %6\n"
                "psubsb\t" "%6, %5\n"
                "paddusb\t" "%5, %1\n"
                "pmovmskb\t" "%1, %2"
                :"=x"(cmp_ret_sh), "=x"(store), "=r"(cur_ret), "=x"(a1), "=x"(a2), "=x"(a3), "=x"(a4)
                :"r"(c + 16 * (i + 1)), "0"(whitespace_reg), "4"(cmp_ret), "5"(_mm_set_epi32(0, 0, 0, 0)), "1"(store));
        

        if (cur_ret != 0) {
            __asm__("psadbw\t" "%3, %0\n"
                    "movd\t"   "%0, %2\n"
                    "movhlps\t" "%0, %0\n"
                    "movd\t" "%0, %1\n"
                    :"=x" (tt), "=r"(high), "=r"(low), "=x"(abs_low)
                    :"0"(_mm_set_epi32(0, 0, 0, 0)), "3"(store)
                    :"0");
            
            ret += high + low;
            store = _mm_set_epi32(0, 0, 0, 0);
        }
        
    }
    
    __asm__("psadbw\t" "%3, %0\n"
            "movd\t"   "%0, %2\n"
            "movhlps\t" "%0, %0\n"
            "movd\t" "%0, %1\n"
            :"=x" (tt), "=r"(high), "=r"(low), "=x"(abs_low)
            :"0"(_mm_set_epi32(0, 0, 0, 0)), "3"(store)
            :"0");
    
    ret += high + low;
    store = _mm_set_epi32(0, 0, 0, 0);

	    
    size_t size16 = sz - sz % 16;    
    if(c[size16 - 1] == ' ' && c[size16] != ' ')  ret--;
    
    for (int i = size16; i < sz; i++)  {
        if (c[i] != ' ' && c[i - 1] == ' ')
        	ret++;
    }
    
    return ret;
}

void test() {
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			std::string left(i, ' ');
			std::string right(j, ' ');
			std::string mid = "asdhkjashdkjsal    ajskldj aslkdj asld jaksl djakls djkas d asdkj asldkj aslkdj askld alsk dlkas d       asdas"\
			"ajsdkljasldkjasdjalskd aksj dkaslj iqwue oiqwue iqiwoeaskd k             alksdjaks dl          kasjd sald           ajslkdjsalk";
			std::string to_test = left + mid + right;
			if (trivial_count(to_test) != count(to_test)) {
				std::cout << "Tests failed\n";
				return;
			}
		}
	}
	std::cout << "Tests success\n";
}

int main() {
	test();
    return 0;
}