#pragma once
#include "../../Memory/Memory.h"

class MinecraftGame {
public:
	BUILD_ACCESS(bool, mouseGrabbed, 0x1A8);
	//BUILD_ACCESS(Font*, mcFont, 0xF70); not needed as of now
}; 