#pragma once
#include "Player.h"

class LocalPlayer : public Player {
public:
    void resetRot() {
        Memory::callVirtualFuncSolstice<void>(
            VTables::resetRot, this, "resetRot");
    }
};
