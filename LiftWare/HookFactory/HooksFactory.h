#pragma once
#include <string>
#include <vector>
#include "FuncHook/FuncHook.h"
#include "../../Memory/Memory.h"
#include <MinHook.h>
#include "../SDK/Game.h"

class HooksFactory {
public:
	static void init();
	static void shutdown();
public:
	static inline std::vector<FuncHook*> hooksCache;

	template<typename Hook>
	static void RequestHook(uintptr_t address) {
		if (!address)
			return;

		Hook* funcHook = new Hook();
		funcHook->name = typeid(Hook).name();
		funcHook->address = address;
		funcHook->onHookRequest();

		hooksCache.push_back((FuncHook*)funcHook);
	}


	template<typename Hook>
	static void RequestHook(std::string_view sig) {
		RequestHook<Hook>(Memory::FindSignature(sig));
	}

	template<typename Hook>
	static void RequestHook(uintptr_t** VTable, int index) {
		RequestHook<Hook>((uintptr_t)VTable[index]);
	}

	template<typename Hook>
	static void RequestRefHook(std::string_view sig) {
		RequestHook<Hook>(Memory::ResolveRef(Memory::FindSignature(sig)));
	}

	template<typename TRet>
	static TRet* getHook() {
		for (FuncHook* funcHook : hooksCache) {
			TRet* result = dynamic_cast<TRet*>(funcHook);
			if (result == nullptr)
				continue;
			return result;
		}
		return nullptr;
	}
};