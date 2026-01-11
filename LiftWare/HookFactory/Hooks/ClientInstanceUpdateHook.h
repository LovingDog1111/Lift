#pragma once
#include "../FuncHook/FuncHook.h"

class ClientInstanceUpdateHook : public FuncHook {
private:
	using func_t = __int64(__thiscall*)(ClientInstance*, bool); //oh lord i gotta add these
	static inline func_t oFunc;

	static __int64 ClientInstance_UpdateCallback(ClientInstance* _this, bool a2) {
		Game::clientInstance = _this;
		return oFunc(_this, a2);
	}
public:
	ClientInstanceUpdateHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ClientInstance_UpdateCallback;
	}
};
