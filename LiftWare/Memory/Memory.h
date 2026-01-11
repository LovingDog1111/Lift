#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <string_view>
#include <cstdint>
#include "../Values/Offsets.h"
#include "../Math/IncludeAll.h"

#define BUILD_ACCESS(type, name, offset)																			\
__declspec(property(get = get##name, put = set##name)) type name;													\
inline type& get##name() { return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); }			\
inline void set##name(type v) { *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset) = v; }

namespace Memory {
    uintptr_t GetGameBase();
    uintptr_t FindSignature(std::string_view sig, std::string_view name = "");
    uintptr_t** GetVTableFromSignature(std::string_view sig);
	uintptr_t** getVtableFromSigSolstice(std::string_view sig);
    template <unsigned int I, typename R, typename... A>
    inline R CallVFunc(void* p, A... a) {
        return (*reinterpret_cast<R(__thiscall***)(void*, A...)>(p))[I](p, a...);
    }

	template<typename Ret, typename... Args>
	inline Ret callVirtualFuncSolstice(int index, void* _this, const char* vtableName, Args... args) {
		using Fn = Ret(__thiscall*)(void*, Args...);
		auto vtable = *reinterpret_cast<uintptr_t**>(_this);

		if (!vtable || !vtable[index]) {
			std::cout << "[vfunc] Call failed at vtable '" << vtableName
				<< "', index " << index << std::endl;
			if constexpr (!std::is_void_v<Ret>) {
				return Ret(); 
			}
			else {
				return; 
			}
		}

		if constexpr (std::is_void_v<Ret>) {
			reinterpret_cast<Fn>(vtable[index])(_this, args...);
			//std::cout << "[vfunc] Call succeeded at vtable '" << vtableName
				//<< "', index " << index << std::endl;
		}
		else {
			Ret result = reinterpret_cast<Fn>(vtable[index])(_this, args...);
			//std::cout << "[vfunc] Call succeeded at vtable '" << vtableName
				//<< "', index " << index << std::endl;
			return result;
		}
	}

    template <typename R, typename... A>
    inline R CallFunc(void* f, A... a) {
        return reinterpret_cast<R(*)(A...)>(f)(a...);
    }

    template <typename R, typename... A>
    inline R CallFunc(uintptr_t f, A... a) {
        return reinterpret_cast<R(*)(A...)>(f)(a...);
    }

	template <typename TRet, typename... TArgs>
	static TRet callFastcall(uintptr_t func, TArgs... args)
	{
		using Fn = TRet(__fastcall*)(TArgs...);
		Fn f = reinterpret_cast<Fn>(func);
		return f(args...);
	}

    inline uintptr_t getFuncFromCall(uintptr_t address) {
        if (address == 0)
            return 0;

        return address + 1 + 4 + *(int*)(address + 1);
    }

    inline uintptr_t ResolveCall(uintptr_t a) {
        return a ? a + 5 + *reinterpret_cast<int*>(a + 1) : 0;
    }

	inline void nopBytes(void* dst, unsigned int size) {
		DWORD oldprotect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memset(dst, 0x90, size);
		VirtualProtect(dst, size, oldprotect, &oldprotect);
	}

	inline void copyBytes(void* src, void* dst, unsigned int size) {
		DWORD oldprotect;
		VirtualProtect(src, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy(dst, src, size);
		VirtualProtect(src, size, oldprotect, &oldprotect);
	}

	inline void patchBytes(void* dst, void* src, unsigned int size) {
		DWORD oldprotect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy(dst, src, size);
		VirtualProtect(dst, size, oldprotect, &oldprotect);
	}

	template <typename T>
	inline T read(uintptr_t address) {
		if (!address)
			return T(); 
		return *reinterpret_cast<T*>(address);
	}

	template <typename T>
	inline T* readPtr(uintptr_t address) {
		if (!address)
			return nullptr;
		return reinterpret_cast<T*>(address);
	}

    static uintptr_t ResolveRef(uintptr_t addr) {
        if (!addr)
            return 0;

        unsigned char* bytes = reinterpret_cast<unsigned char*>(addr);

        if (bytes[0] == 0xE8 || bytes[0] == 0xE9)
            return addr + 5 + *reinterpret_cast<int*>(addr + 1);

        if (
            (bytes[0] == 0x48 && bytes[1] == 0x8D && (bytes[2] == 0x05 || bytes[2] == 0x0D)) ||
            (bytes[0] == 0x4C && bytes[1] == 0x8D && bytes[2] == 0x25)
            )
            return addr + 7 + *reinterpret_cast<int*>(addr + 3);

        return addr;
    }
}

static const char* const KeyNames[] = {
	"Unknown",
	"VK_LBUTTON",
	"VK_RBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"Unknown",
	"Backspace",
	"Tab",
	"Unknown",
	"Unknown",
	"VK_CLEAR",
	"VK_RETURN",
	"Unknown",
	"Unknown",
	"Shift",
	"Ctrl",
	"Alt",
	"VK_PAUSE",
	"VK_CAPITAL",
	"VK_KANA",
	"Unknown",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI",
	"Unknown",
	"VK_ESCAPE",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",
	"VK_SPACE",
	"VK_PRIOR",
	"VK_NEXT",
	"VK_END",
	"VK_HOME",
	"VK_LEFT",
	"VK_UP",
	"VK_RIGHT",
	"VK_DOWN",
	"VK_SELECT",
	"VK_PRINT",
	"VK_EXECUTE",
	"VK_SNAPSHOT",
	"Insert",
	"Delete",
	"VK_HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"VK_LWIN",
	"VK_RWIN",
	"VK_APPS",
	"Unknown",
	"VK_SLEEP",
	"NUMPAD0",
	"NUMPAD1",
	"NUMPAD2",
	"NUMPAD3",
	"NUMPAD4",
	"NUMPAD5",
	"NUMPAD6",
	"NUMPAD7",
	"NUMPAD8",
	"NUMPAD9",
	"VK_MULTIPLY",
	"VK_ADD",
	"VK_SEPARATOR",
	"VK_SUBTRACT",
	"VK_DECIMAL",
	"VK_DIVIDE",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"VK_F13",
	"VK_F14",
	"VK_F15",
	"VK_F16",
	"VK_F17",
	"VK_F18",
	"VK_F19",
	"VK_F20",
	"VK_F21",
	"VK_F22",
	"VK_F23",
	"VK_F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_NUMLOCK",
	"VK_SCROLL",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"LeftShift",
	"RightShift",
	"VK_LCONTROL",
	"VK_RCONTROL",
	"VK_LMENU",
	"VK_RMENU",
	"Browser Back",
	"Browser Forward",
	"Browser Refresh",
	"Browser Stop",
	"Browser Search",
	"Browser Favourites",
	"Browser Home",
	"Volume Mute",
	"Volume Down",
	"Volume Up",
	"Media_Next",
	"Media_Prev",
	"Media_Stop",
	"Media_Pause",
	"Mail",
	"Media",
	"App1",
	"App2",
	"Unknown",
	"Unknown",
	"OEM_1",
	"PLUS",
	"COMMA",
	"MINUS",
	"DOT",
	"OEM_2",
	"OEM_3"
};