#pragma once
#include "../../Values/Pads.h"

class Minecraft {
private:
	char pad_0x0[Pads::Minecraft_pad_0x0];
public:
	float* minecraftTimer;
	float* minecraftRenderTimer;
};