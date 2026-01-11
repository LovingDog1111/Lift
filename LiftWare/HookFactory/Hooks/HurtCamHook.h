#pragma once
#include "../FuncHook/FuncHook.h"
#include "../../Lift/Lift.h"
#include "../../Lift/Features/FeatureFactory.h"
#include "../../Lift/Commands/CommandFactory.h"

class HurtCamHook : public FuncHook {
private:
	using func_t = __int64(__thiscall*)(Actor*, char, uint32_t);
	static inline func_t oFunc;

	static __int64 CauseHurtCamCallback(Actor* a1, char a2, uint32_t a3) {
		__int64 result = oFunc(a1, a2, a3);

		static NoHurtCam* noHurtCamMod = FeatureFactory::getFeature<NoHurtCam>();

		if (noHurtCamMod->isEnabled() && a2 == 2 && a1 == Game::getLocalPlayer()) {
			uint32_t* a4 = *(uint32_t**)((uintptr_t)a1 + 0x420); // MobAnimation::decrementHurtTime
			if (a4 != nullptr) {
				*a4 = 0;
			}
		}

		return result;
	}
public:
	HurtCamHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&CauseHurtCamCallback;
	}
	// search for "minecraft:totem_particle" lel
};