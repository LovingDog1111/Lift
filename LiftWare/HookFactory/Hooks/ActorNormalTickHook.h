#pragma once
#include "../FuncHook/FuncHook.h"
#include "../../Lift/Lift.h"

class ActorNormalTickHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(Actor*);
	static inline func_t oFunc;
	static inline bool hasAnnounced = false;

	static void ActorNormalTickCallback(Actor* _this) {
		LocalPlayer* localPlayer = Game::getLocalPlayer();
		if (localPlayer == _this) {
			FeatureFactory::onNormalTick(localPlayer);
			if (!hasAnnounced) {
				Lift::displayMessage("Here are the available modules: Test, Arraylist, KillAura. The commands you can use are Bind and SetPos. Type .bind (module) (key) to assign a key.");
				hasAnnounced = true;
			}
		}
		else {
			hasAnnounced = false;
		}
		oFunc(_this);
	}
public:
	ActorNormalTickHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ActorNormalTickCallback;
	}
};
