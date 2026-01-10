#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <string_view>
#include <cstdint>

#define BUILD_ACCESS(type, name, offset)																			\
__declspec(property(get = get##name, put = set##name)) type name;													\
inline type& get##name() { return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); }			\
inline void set##name(type v) { *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset) = v; }

namespace Memory {
    uintptr_t GetGameBase();
    uintptr_t FindSignature(std::string_view sig, std::string_view name = "");
    uintptr_t** GetVTableFromSignature(std::string_view sig);

    template <unsigned int I, typename R, typename... A>
    inline R CallVFunc(void* p, A... a) {
        return (*reinterpret_cast<R(__thiscall***)(void*, A...)>(p))[I](p, a...);
    }

    template <typename R, typename... A>
    inline R CallFunc(void* f, A... a) {
        return reinterpret_cast<R(*)(A...)>(f)(a...);
    }

    template <typename R, typename... A>
    inline R CallFunc(uintptr_t f, A... a) {
        return reinterpret_cast<R(*)(A...)>(f)(a...);
    }

    inline uintptr_t getFuncFromCall(uintptr_t address) {
        if (address == 0)
            return 0;

        return address + 1 + 4 + *(int*)(address + 1);
    }

    inline uintptr_t ResolveCall(uintptr_t a) {
        return a ? a + 5 + *reinterpret_cast<int*>(a + 1) : 0;
    }

    inline void Patch(void* d, void* s, size_t n) {
        DWORD o;
        VirtualProtect(d, n, PAGE_EXECUTE_READWRITE, &o);
        memcpy(d, s, n);
        VirtualProtect(d, n, o, &o);
    }

    inline void Nop(void* d, size_t n) {
        DWORD o;
        VirtualProtect(d, n, PAGE_EXECUTE_READWRITE, &o);
        memset(d, 0x90, n);
        VirtualProtect(d, n, o, &o);
    }
}
