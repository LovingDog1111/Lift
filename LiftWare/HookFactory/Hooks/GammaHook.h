#pragma once
#include "../FuncHook/FuncHook.h"

class GammaHook : public FuncHook {
private:
	using func_t = float(__thiscall*)(__int64);
	static inline func_t oFunc;

	static float GetGammaCallback(__int64 a1) {

		static bool initPtr = false; //we dont have module manager yet.
		if (!initPtr) {
			uintptr_t** list = (uintptr_t**)a1;
			initPtr = true;
		}

		return 12.f; //lets see if this works. if im right, when we inject the client, gamma should always be this number
		return oFunc(a1);
	}
public:
	GammaHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&GetGammaCallback;
	}
};