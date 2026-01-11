#pragma once
#include "../FuncHook/FuncHook.h"

class ActorNormalTickHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(Actor*);
	static inline func_t oFunc;

	static void ActorNormalTickCallback(Actor* _this) {
		LocalPlayer* localPlayer = Game::getLocalPlayer();
		if (localPlayer == _this) {
			FeatureFactory::onNormalTick(localPlayer); //YAYAYAY! :3
		}
		oFunc(_this);
	}
public:
	ActorNormalTickHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ActorNormalTickCallback;
	}
};