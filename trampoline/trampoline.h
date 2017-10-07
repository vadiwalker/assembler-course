#ifndef TRAMPOLINE_H
#define TRAMPOLINE_H

#include <stdlib.h>
#include <sys/mman.h>
#include <xmmintrin.h>
#include <iostream>
#include <functional>
#include <cassert>

template <typename ... Args>
struct args_t;

template <>
struct args_t<> {
    static const int INT = 0;
    static const int SSE = 0;
};

template <typename First, typename ... Args>
struct args_t<First, Args ...> {
    static const int INT = args_t<Args ...>::INT + 1;
    static const int SSE = args_t<Args ...>::SSE;
};

template <typename ... Args>
struct args_t<float, Args ...> {
    static const int INT = args_t<Args ...>::INT;
    static const int SSE = args_t<Args ...>::SSE + 1;
};

template <typename ... Args>
struct args_t<double, Args ...> {
    static const int INT = args_t<Args ...>::INT;
    static const int SSE = args_t<Args ...>::SSE + 1;
};

template <typename ... Args>
struct args_t<__m64, Args ...> {
    static const int INT = args_t<Args ...>::INT;
    static const int SSE = args_t<Args ...>::SSE + 1;
};

template <typename T>
struct trampoline;

template<typename R, typename... Args>
void swap(trampoline<R(Args...)>& lhs, trampoline<R(Args...)>& rhs);

template <typename T, typename ... Args>
struct trampoline<T (Args ...)> {

    template <typename FUNC>
    trampoline(FUNC func) : func_obj(new FUNC(std::move(func))), deleter(create_deleter<FUNC>) {
        code = make_malloc();
        char* pcode = (char*)code;
        
        if (args_t<Args ...>::INT <= REGISTERS) {
            for (int i = args_t<Args ...>::INT - 1; i >= 0; i--) add(pcode, offsets[i]);
            add(pcode,"\x48\xbf");
            *(void**)pcode = func_obj;
            pcode += 8;
            add(pcode, "\x48\xb8");
            *(void**)pcode = (void*)&do_call<FUNC>;
            pcode += 8;
            add(pcode, "\xff\xe0");
        } else {
            int stack_size = 8 * (args_t<Args ...>::INT - 5 + std::max(args_t<Args ...>::SSE - 8, 0));
            add(pcode, "\x4c\x8b\x1c\x24");
            for (int i = 5 ; i >= 0; i--) add(pcode, offsets[i]);
            add(pcode, "\x48\x89\xe0\x48\x05");
            *(int32_t*)pcode = stack_size;
            pcode += 4;
            add(pcode,"\x48\x81\xc4");
            *(int32_t*)pcode = 8;
            pcode += 4;
            char* point1 = pcode;
            add(pcode,"\x48\x39\xe0\x74");
            char* point2 = pcode;
            pcode++;
            {
                add(pcode,"\x48\x81\xc4\x08");
                pcode += 3;
                add(pcode, "\x48\x8b\x3c\x24\x48\x89\x7c\x24\xf8\xeb");
                *pcode = point1 - pcode - 1;
                pcode++;
            }
            *point2 = pcode - point2 - 1;
            add(pcode, "\x4c\x89\x1c\x24\x48\x81\xec");
            *(int32_t*)pcode = stack_size;
            pcode += 4;
            add(pcode,"\x48\xbf");
            *(void**)pcode = func_obj;
            pcode += 8;
            add(pcode, "\x48\xb8");
            *(void**)pcode = (void*) &do_call<FUNC>;
            pcode += 8;
            add(pcode,"\xff\xd0\x41\x59\x4c\x8b\x9c\x24");
            *(int32_t*)pcode = stack_size - 8;
            pcode += 4;
            add(pcode, "\x4c\x89\x1c\x24\xc3");
        }
    }
    
    trampoline(trampoline&& other) {
        func_obj = other.func_obj;
        code = other.code;
        deleter = other.deleter;
        other.func_obj = nullptr;
    }
    
    trampoline(const trampoline&) = delete;

    template <class TR>
    trampoline& operator=(TR&& func) {
        trampoline tmp(std::move(func));
        ::swap(*this, tmp);
        return *this;
    }
    
    ~trampoline() {
        if (func_obj) deleter(func_obj);
        *(void **) code = pointer;
        pointer = (void **) code;
    }
    
    T (*get() const)(Args ... args) {
        return (T(*)(Args ... args))code;
    }
    
    void swap(trampoline &other) {
        ::swap(*this, other);
    }
    
private:

    void *pointer = nullptr;
    const size_t PAGE_SIZE = 4096;
    const size_t SIZE_FUNC = 255;
    static const int REGISTERS = 6;

    void* allocate_page() {
        char* page = static_cast<char*>(mmap(nullptr, PAGE_SIZE, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
        if (page == MAP_FAILED) return nullptr;
        *page = 0;
        page += sizeof(int);
        for (size_t i = 0; i < PAGE_SIZE - sizeof(int); i += SIZE_FUNC)
            *reinterpret_cast<char**>(page + i - (i == 0 ? 0 : SIZE_FUNC)) = page + i;
        return page;
    }

    bool shift_counter(void *page) {
        void *page_start = (void*)(((size_t)page >> 12) << 12);
        *static_cast<int*>(page_start) += 1;
        if (*static_cast<int*>(page_start) == 0) {
            munmap(page_start, PAGE_SIZE);
            return false;
        }
        return true;
    }

    void* make_malloc() {
        if (!pointer) {
            pointer = allocate_page();
            if (!pointer) {
                return nullptr;
            }
        }
        shift_counter(pointer);
        void *ret = pointer;
        pointer = *reinterpret_cast<void**>(pointer);
        return ret;
    }

    friend void ::swap<>(trampoline& a, trampoline& b);

    const char* offsets[REGISTERS] = {
        "\x48\x89\xfe","\x48\x89\xf2",
        "\x48\x89\xd1","\x49\x89\xc8",
        "\x4d\x89\xc1","\x41\x51"
    };
    
    void add(char* &p, const char* command) {
        for (const char *i = command; *i; i++) 
            *(p++) = *i;
    }
    
    template <typename F>
    static T do_call(void* obj, Args ...args) {
        return (*static_cast<F*>(obj))(std::forward<Args>(args)...);
    }
    
    template <typename F>
    static void create_deleter(void* func_obj) {
        delete static_cast<F*>(func_obj);
    }
    
    void* func_obj;
    void* code;
    void (*deleter)(void*);
};

template<typename R, typename... Args>
void swap(trampoline<R(Args...)>& lhs, trampoline<R(Args...)>& rhs) {
    std::swap(lhs.func_obj, rhs.func_obj);
    std::swap(lhs.code, rhs.code);
    std::swap(lhs.deleter, rhs.deleter);
}

#endif