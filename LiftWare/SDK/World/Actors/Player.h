#pragma once
#include "Mob.h"
#include "../GameMode/GameMode.h"

class Player : public Mob {
public:
	BUILD_ACCESS(GameMode*, gameMode, 0xEC8);
public:
	void playEmote(std::string const& yes, bool t) {
		return Memory::CallVFunc<70, void, std::string const&, bool>(this, yes, t);
	}
	int getItemUseDuration() {
		return Memory::CallVFunc<162, int>(this);
	}
};
