#pragma once
#include "../../Math/IncludeAll.h"
#include "../../Values/Pads.h"
#include "../../Memory/Memory.h"
#include "../MCTextFormat.h"

class GuiData {
private:
	char pad_0x0[0x40];
public:
	Vector2<float> windowSizeReal;
	Vector2<float> windowSizeReal2;
	Vector2<float> windowSizeScaled;
	double guiScale1;
	float guiScale2;

	void displayMessage(const std::string& msg)//std::optional<std::string> const& sender
	{
		MCOptional mcSender = MCOptional::nullopt();
		static auto address = Memory::FindSignature("40 55 53 56 57 41 56 48 8D AC 24 A0 FE FF FF 48 81 EC 60 02 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 50 01 00 00 41");
		Memory::callFastcall<void>(address, this, msg, mcSender, false);
	}
};