#pragma once
#include "../FuncHook/FuncHook.h"

class ClientInstanceUpdateHook : public FuncHook {
private:
	using func_t = __int64(__thiscall*)(ClientInstance*, bool); //oh lord i gotta add these
	static inline func_t oFunc;

	static __int64 ClientInstance_UpdateCallback(ClientInstance* _this, bool a2) {
		Game::clientInstance = _this;
		//backup
		if (!FeatureFactory::getFeature<ClickGUI>()->initialized) {
			FeatureFactory::getFeature<ClickGUI>()->InitClickGUI();
		}
		return oFunc(_this, a2);
	}
public:
	ClientInstanceUpdateHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ClientInstance_UpdateCallback;
	}
};
