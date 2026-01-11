#pragma once
#include "Actor.h"
#include "../../Values/Vtables.h"

class Mob : public Actor {
public:
    void setSprinting(bool shouldSprint) {
        Memory::callVirtualFuncSolstice<void, bool>(
            VTables::MobSetSprinting, this, "MobSetSprinting", shouldSprint);
    }
};

