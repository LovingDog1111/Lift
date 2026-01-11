#pragma once
#include "../FuncHook/FuncHook.h"
#include "../../MainClient/Lift.h"
#include "../../MainClient/Features/FeatureFactory.h"
#include "../../MainClient/Commands/CommandFactory.h"

class KeyMapHook : public FuncHook {
private:
	using func_t = __int64(__fastcall*)(uint64_t, bool);
	static inline func_t oFunc;

	static __int64 KeyInputCallback(uint64_t key, bool isDown) {

		if (Game::canUseMoveKeys())
			FeatureFactory::onKeyUpdate((int)key, isDown);

		return oFunc(key, isDown);
	}
public:
	static void createKeyPress(uint64_t key, bool down) {
		oFunc(key, down);
	}

	KeyMapHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&KeyInputCallback;
	}

	void onHookRequest() override {
		uintptr_t keyMapOffset = 0x0;
		uintptr_t sigOffset = this->address + 7;
		int offset = *reinterpret_cast<int*>((sigOffset + 3));
		keyMapOffset = sigOffset + offset + 7;
		Game::keyMapPtr = (void*)keyMapOffset;
	}
};