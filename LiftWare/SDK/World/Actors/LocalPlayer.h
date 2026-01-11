#pragma once
#include "Player.h"

class LocalPlayer : public Player {
public:
	void displayClientMessage(const std::string& message) {
		Memory::CallVFunc<VTables::LocalPlayerDisplayClientMessage, void, const std::string&>(this, message);
	}
};
