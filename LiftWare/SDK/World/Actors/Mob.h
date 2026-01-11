#pragma once
#include "Actor.h"
#include "../../Values/Vtables.h"

class Mob : public Actor {
public:
	void setSprinting(bool shouldSprint) {
		Memory::CallVFunc<VTables::MobSetSprinting, void, bool>(this, shouldSprint);
	}
};
