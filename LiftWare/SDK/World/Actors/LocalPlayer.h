#pragma once
#include "Player.h"

class LocalPlayer : public Player {
public:
    void displayClientMessage(const std::string& message) {
        Memory::callVirtualFuncSolstice<void, const std::string&>(
            VTables::LocalPlayerDisplayClientMessage, this, "LocalPlayerDisplayClientMessage", message);
    }

    void resetRot() {
        Memory::callVirtualFuncSolstice<void>(
            VTables::resetRot, this, "resetRot");
    }
};
