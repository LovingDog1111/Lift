#pragma once
#include "Mob.h"
#include "../GameMode/GameMode.h"
#include "../../Values/Vtables.h"

class Player : public Mob {
public:
	BUILD_ACCESS(GameMode*, gameMode, Offsets::GameMode);
public:
	void playEmote(std::string const& yes, bool t) {
		return Memory::CallVFunc<VTables::PlayerPlayEmote, void, std::string const&, bool>(this, yes, t);
	}
	int getItemUseDuration() {
		return Memory::CallVFunc<VTables::PlayerGetItemUseDuration, int>(this);
	}
};
