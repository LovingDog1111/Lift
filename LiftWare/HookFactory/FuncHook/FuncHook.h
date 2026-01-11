#pragma once
#include <string>
#include <memory>
#include "../../Logs/Logger.h"
#include "../../SDK/Game.h"
#include "../../Lift/Features/FeatureFactory.h"

class FuncHook {
public:
	const char* name;
	uintptr_t address;
	void* OriginFunc;
	void* func;
	bool enable = false;

	virtual bool enableHook();
	virtual void onHookRequest();
};
