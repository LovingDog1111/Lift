#pragma once
#include "Mob.h"
#include "../GameMode/GameMode.h"
#include "../../Values/Vtables.h"

class Player : public Mob {
public:
	BUILD_ACCESS(GameMode*, gameMode, Offsets::GameMode);
public:
    void playEmote(std::string const& yes, bool t) {
        Memory::callVirtualFuncSolstice<void, std::string const&, bool>(
            VTables::PlayerPlayEmote, this, "PlayerPlayEmote", yes, t);
    }
    int getItemUseDuration() {
        return Memory::callVirtualFuncSolstice<int>(
            VTables::PlayerGetItemUseDuration, this, "PlayerGetItemUseDuration");
    }

};
