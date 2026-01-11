#pragma once
#include "../../Memory/Memory.h"
#include "../../Values/Vtables.h"

class MinecraftGame {
public:
	BUILD_ACCESS(bool, mouseGrabbed, Offsets::MouseGrabbed);
	
	void playUI(std::string name, float volume, float pitch)
	{
		if (this == nullptr)
			return;

		Memory::callVirtualFuncSolstice<void, std::string, float, float>(
			VTables::PlayUI, this, "PlayUI", name, volume, pitch);
	}
}; 